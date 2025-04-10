#include <Arduino.h>
#include "model_params.h"  // 学習済み線形SVMのパラメータ：INTERCEPT, COEFFICIENTS が定義されている

// センサに関するピン設定
const int sensor_pin[6] = { 34, 35, 32, 33, 25, 26 };
const int sensor_ctl_pin[3] = { 27, 14, 12 };
int sensor_value[3][6];  // 3行×6列＝18個のセンサーデータ
int template_;
int isSend;  // 物体検出判定用フラグ

// SVM推論用定数
#define NUM_FEATURES 18  // センサーデータは18個
#define NUM_CLASSES 4    // 分類クラスは 0～3 の4種

// 線形SVMによる推論処理関数
// 各クラスについてスコア = INTERCEPT + Σ( COEFFICIENTS * feature ) を計算し、
// 最大のスコアを示すクラスを予測結果として返す
int predictObject(const float features[NUM_FEATURES]) {
  float scores[NUM_CLASSES] = { 0.0 };
  
  // 各クラスのスコア計算
  for (int i = 0; i < NUM_CLASSES; i++) {
    scores[i] = INTERCEPT[i];
    for (int j = 0; j < NUM_FEATURES; j++) {
      scores[i] += COEFFICIENTS[i][j] * features[j];
    }
  }
  
  // 最大スコアをもつクラスを決定
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
  delay(1000);  // シリアルモニタ用の待機
  Serial.println("start...");
  
  // センサ制御ピンの設定
  for (int i = 0; i < 3; i++ ) {
    pinMode(sensor_ctl_pin[i], OUTPUT);
    digitalWrite(sensor_ctl_pin[i], HIGH);
  }
  
  // センサ入力ピンの設定
  for (int i = 0; i < 6; i++ ) {
    pinMode(sensor_pin[i], INPUT);
  }
}

void loop() {
  // センサデータ取得処理
  for (int i = 0; i < 3; i++) {
    digitalWrite(sensor_ctl_pin[i], LOW);
    delay(10);  // トランジスタ安定化待ち

    for (int j = 0; j < 6; j++) {
      template_ = analogRead(sensor_pin[j]);
      sensor_value[i][j] = template_;
      delay(10);
    }

    digitalWrite(sensor_ctl_pin[i], HIGH);
  }

  // 物体検出判定（ここでは常に送信とする）
  isSend = true;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 6; j++) {
      if (sensor_value[i][j] < 400) {
        isSend = true;
      }
    }
  }
  
  if (isSend == true) {
    // 取得した3×6のセンサデータを1次元の配列へ再構成
    float features[NUM_FEATURES];
    int idx = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 6; j++) {
        features[idx++] = sensor_value[i][j];
      }
    }
    
    // SVMによる推論の実施
    int predictedClass = predictObject(features);

    // シリアル出力（フォーマット例：5,＜18データ＞,予測クラス,6）
    Serial.print("5,");
    for (int i = 0; i < 3; i++) {
      Serial.print(sensor_value[i][0]); Serial.print(",");
      Serial.print(sensor_value[i][1]); Serial.print(",");
      Serial.print(sensor_value[i][2]); Serial.print(",");
      Serial.print(sensor_value[i][3]); Serial.print(",");
      Serial.print(sensor_value[i][4]); Serial.print(",");
      Serial.print(sensor_value[i][5]); Serial.print(",");
    }
    Serial.println(",6");
    
    // またはシリアルモニタに推論結果を表示
    Serial.print("Predicted Object Class: ");
    Serial.println(predictedClass);
  }
  
  delay(100);
}
