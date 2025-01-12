const int sensor_pin[6] = { A0, A1, A2, A3, A4, A5};
const int sensor_ctl_pin[3] = { 8, 9, 10};
int sensor_value[3][6];
int template_;
int L13_sta;
int isSend;

void setup() {
  // put your setup code here, to run once:

  L13_sta = HIGH;
  pinMode(13, OUTPUT);
  digitalWrite(13, L13_sta);

  for( int i=0; i<3; ++i ){
    pinMode( sensor_ctl_pin[i], OUTPUT);
    digitalWrite( sensor_ctl_pin[i], HIGH);
  }

  for( int i=0; i<6; ++i ){
    pinMode( sensor_pin[i], INPUT);
  }
  
  Serial.begin(9600);
  Serial.println("start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for( int i=0; i<3; ++i ){

    digitalWrite( sensor_ctl_pin[i], LOW);
    delay(10); // トランジスタの安定待ち

    for( int j=0; j<6; ++j ){

      template_ = analogRead( sensor_pin[j] );
      if( template_ < 100 ) template_= 0;
      else  template_ = 1;
      sensor_value[i][j] = template_;
      delay(10);
    }

    digitalWrite( sensor_ctl_pin[i], HIGH);
  }

  // 物体があるかの確認
  isSend = false;
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
  if( L13_sta == HIGH ) L13_sta = LOW;
  else  L13_sta = HIGH;
  digitalWrite(13, L13_sta);

  delay(1000);
}
