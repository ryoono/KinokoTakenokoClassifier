#include <Arduino.h>
#include "model_params.h"  // INTERCEPT, COEFFICIENTS 定義

const int sensor_pin[6] = { 34, 35, 32, 33, 25, 26 };
const int sensor_ctl_pin[3] = { 27, 14, 12 };
int sensor_value[3][6];
int sensor_value_compression[3][6];
int template_;
int isSend;
const int takenoko_pin = 17;
const int kinoko_pin = 16;

#define NUM_FEATURES 18
#define NUM_CLASSES 4

int predictObject(const float features[NUM_FEATURES]) {
  float scores[NUM_CLASSES] = { 0.0 };
  for (int i = 0; i < NUM_CLASSES; i++) {
    scores[i] = INTERCEPT[i];
    for (int j = 0; j < NUM_FEATURES; j++) {
      scores[i] += COEFFICIENTS[i][j] * features[j];
    }
  }
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
  delay(1000);
  Serial.println("start...");

  for (int i = 0; i < 3; ++i ) {
    pinMode(sensor_ctl_pin[i], OUTPUT);
    digitalWrite(sensor_ctl_pin[i], HIGH);
  }
  for (int i = 0; i < 6; ++i ) {
    pinMode(sensor_pin[i], INPUT);
  }

  pinMode(takenoko_pin, OUTPUT); digitalWrite(takenoko_pin, HIGH);
  pinMode(kinoko_pin, OUTPUT); digitalWrite(kinoko_pin, HIGH);
}

void loop() {
  for (int i = 0; i < 3; ++i) {
    digitalWrite(sensor_ctl_pin[i], LOW);
    delay(5);
    for (int j = 0; j < 6; ++j) {
      template_ = analogRead(sensor_pin[j]);
      sensor_value[i][j] = template_;
      if (template_ >= 1000) sensor_value_compression[i][j] = 10;
      else sensor_value_compression[i][j] = template_ / 100;
      delay(5);
    }
    digitalWrite(sensor_ctl_pin[i], HIGH);
  }

  isSend = false;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (sensor_value_compression[i][j] < 1)
        isSend = true;
    }
  }

  float features[NUM_FEATURES];
  int idx = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 6; ++j) {
      features[idx++] = sensor_value_compression[i][j];
    }
  }

  // ----------- 推論時間の測定開始 -----------
  unsigned long startMicros = micros();
  int predictedClass = predictObject(features);
  unsigned long endMicros = micros();
  unsigned long inferenceTime = endMicros - startMicros;
  // -------------------------------------------

  // LED制御（従来通り）
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

  // ----------- 推論時間のみを出力 -----------
  Serial.print("Inference time (us): ");
  Serial.println(inferenceTime);
  // -------------------------------------------

  // 元のセンサーデータ送信部分（コメントアウト）
  /*
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
  */

  delay(25);
}
