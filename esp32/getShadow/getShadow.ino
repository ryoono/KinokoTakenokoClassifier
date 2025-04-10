const int sensor_pin[6] = { 34, 35, 32, 33, 25, 26};
const int sensor_ctl_pin[3] = { 27, 14, 12};
int sensor_value[3][6];
int template_;
int L13_sta;
int isSend;

void setup() {
  // put your setup code here, to run once:

//  L13_sta = HIGH;
//  pinMode(13, OUTPUT);
//  digitalWrite(13, L13_sta);

  for( int i=0; i<3; ++i ){
    pinMode( sensor_ctl_pin[i], OUTPUT);
    digitalWrite( sensor_ctl_pin[i], HIGH);
  }

  for( int i=0; i<6; ++i ){
    pinMode( sensor_pin[i], ANALOG);
  }
  
  Serial.begin(115200);
  Serial.println("start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for( int i=0; i<3; ++i ){

    digitalWrite( sensor_ctl_pin[i], LOW);
    delay(10); // トランジスタの安定待ち

    for( int j=0; j<6; ++j ){

      template_ = analogRead( sensor_pin[j] );
//      if( template_ < 100 ) template_= 0;
//      else  template_ = 1;
      sensor_value[i][j] = template_;
      delay(10);
    }

    digitalWrite( sensor_ctl_pin[i], HIGH);
  }

  // 物体があるかの確認
//  isSend = false;
  isSend = true;
  for( int i=0; i<3; ++i ){
    for( int j=0; j<6; ++j ){
      if( sensor_value[i][j] == 0 ) isSend = true;
    }
  }

  // 物体がある場合のみ結果表示
  if( isSend == true ){
    Serial.print("5,");
    for( int i=0; i<3; ++i ){
      Serial.print(sensor_value[i][0]);
      Serial.print(",");
      Serial.print(sensor_value[i][1]);
      Serial.print(",");
      Serial.print(sensor_value[i][2]);
      Serial.print(",");
      Serial.print(sensor_value[i][3]);
      Serial.print(",");
      Serial.print(sensor_value[i][4]);
      Serial.print(",");
      Serial.print(sensor_value[i][5]);
      Serial.print(",");
    }
    Serial.println("6");
  }

  // マイコンが暴走していないかの確認LED
//  if( L13_sta == HIGH ) L13_sta = LOW;
//  else  L13_sta = HIGH;
//  digitalWrite(13, L13_sta);

  delay(100);
}


///**
// * 繋ぎ方：http://arms22.blog91.fc2.com/blog-entry-416.html
// * データシート：http://akizukidenshi.com/download/s9648.pdf
// * 
// * カソード側（足が短い方）に電源を、アノード側（足が長い方）に読み取りピンを刺す
// * ※普通のLEDとは違うので注意！！
// */
//
//const int voutPin = 25;
//const int VOLT = 3.3; // 3.3Vを電源とした場合
//const int ANALOG_MAX = 4096; // ESP32の場合
//
//void setup() {
//  Serial.begin(115200);
//}
//
//void loop() {
//  // R1の電圧を取得
//  int reading = analogRead(voutPin);
//
//  // AD値をmVに変換
//  float voltage = ((long)reading * VOLT * 1000) / ANALOG_MAX;
//
//  Serial.print(voltage);
//  Serial.print(" mV, ");
//
//  // 電圧から電流を求める I=E/R (R=1000)
//  float microamp = (voltage * 1000) / 1000;
//
//  Serial.print(microamp);
//  Serial.print(" uA, ");
//
//  // 電流をlxに変換
//  float lx = microamp / (290 / 100);
//
//  Serial.print(lx);
//  Serial.println(" lx");
//
//  delay(1000);
//}
