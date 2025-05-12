#include <Arduino.h>
#include "model_params.h"  // 学習済みロジスティック回帰のパラメータ（INTERCEPT, COEFFICIENTS）が定義されている

// センサに関するピン設定
const int sensor_pin[6] = { 34, 35, 32, 33, 25, 26 };
const int sensor_ctl_pin[3] = { 27, 14, 12 };
int sensor_value[3][6];  // 3行×6列 = 18個のセンサーデータ
int sensor_value_compression[3][6];  // 3行×6列 = 18個のセンサーデータ_加工後
int template_;
int isSend;  // 物体検出判定用フラグ
const int takenoko_pin = 17;  // たけのこ点灯用のピン
const int kinoko_pin = 16;    // きのこ点灯用のピン

// ロジスティック回帰推論用の定数
#define NUM_FEATURES 18  // センサーデータは18個
#define NUM_CLASSES 4    // 物体クラスは 0～3 の4種

// ロジスティック回帰による推論関数
int predictObject(const float features[NUM_FEATURES]) {
  float scores[NUM_CLASSES] = { 0.0 };
  
  // 各クラスのスコアを計算する： score = INTERCEPT + Σ( COEFFICIENT * feature )
  for (int i = 0; i < NUM_CLASSES; i++) {
    scores[i] = INTERCEPT[i];
    for (int j = 0; j < NUM_FEATURES; j++) {
      scores[i] += COEFFICIENTS[i][j] * features[j];
    }
  }
  
  // 最大スコアのクラスを選ぶ
  int predictedClass = 0;
  float maxScore = scores[0];
  for (int i = 1; i < NUM_CLASSES; i++) {
    if (scores[i] > maxScore) {
      maxScore = scores[i];
      predictedClass = i;
    }
  }
  
  return predictedClass;
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // シリアルモニタの初期化待ち
  Serial.println("start...");
  
  // センサ制御ピンを出力に設定
  for (int i = 0; i < 3; ++i ) {
    pinMode(sensor_ctl_pin[i], OUTPUT);
    digitalWrite(sensor_ctl_pin[i], HIGH);
  }
  
  // センサ入力ピンの設定
  for (int i = 0; i < 6; ++i ) {
    pinMode(sensor_pin[i], INPUT);  // ESP32では analogRead() が可能
  }

  pinMode(takenoko_pin, OUTPUT);
  digitalWrite(takenoko_pin, HIGH);
  pinMode(kinoko_pin, OUTPUT);
  digitalWrite(kinoko_pin, HIGH);
}

void loop() {
  // センサデータ取得処理
  for (int i = 0; i < 3; ++i) {
    digitalWrite(sensor_ctl_pin[i], LOW);
    delay(5);  // トランジスタの安定待機

    for (int j = 0; j < 6; ++j) {
      template_ = analogRead(sensor_pin[j]);
      sensor_value[i][j] = template_;
      if( template_ >= 1000 ) sensor_value_compression[i][j] = 10;
      else                    sensor_value_compression[i][j] = template_ / 100;
      delay(5);
    }

    digitalWrite(sensor_ctl_pin[i], HIGH);
  }

  // 物体検出フラグの設定（ここでは必ず送信する例）
  isSend = false;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (sensor_value_compression[i][j] < 1)
        isSend = true;
    }
  }
  
  // ここでセンサーデータを1次元配列に再構成
  float features[NUM_FEATURES];
  int idx = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 6; ++j) {
      features[idx++] = sensor_value_compression[i][j];
    }
  }
  
  // ロジスティック回帰による推論実施
  int predictedClass = predictObject(features);

  // LEDを点灯させる(常に点灯、物体を検出した場合のみ、検出していない方を消灯)
  if( isSend ){
    if( predictedClass == 0 || predictedClass == 1 ){
      digitalWrite(takenoko_pin, LOW);
      digitalWrite(kinoko_pin, HIGH);
    }
    else{
      digitalWrite(takenoko_pin, HIGH);
      digitalWrite(kinoko_pin, LOW);
    }
  }
  else{
    digitalWrite(takenoko_pin, HIGH);
    digitalWrite(kinoko_pin, HIGH);
  }

  // シリアル出力（フォーマット：開始文字「5,」＋18個のデータ＋末尾に「,予測クラス,6」）
  Serial.print("5,");
  Serial.print(predictedClass);
  for (int i = 0; i < 3; ++i) {
    Serial.print(","); Serial.print(sensor_value[i][0]);
    Serial.print(","); Serial.print(sensor_value[i][1]); 
    Serial.print(","); Serial.print(sensor_value[i][2]);
    Serial.print(","); Serial.print(sensor_value[i][3]);
    Serial.print(","); Serial.print(sensor_value[i][4]);
    Serial.print(","); Serial.print(sensor_value[i][5]);
  }
  Serial.println(",6");
  
  // シリアルモニタに別途結果出力も可能
//  Serial.print("Predicted Object Class: ");
//  Serial.println(predictedClass);
//  Serial.println("");
   
  delay(25);
}
