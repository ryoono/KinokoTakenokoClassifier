
int sensor_send_pin[3];
int sensor_receive_pin[6];
int sensor_receive_val[3][6];

void setup(){
  init_value();

  Serial.begin(9600);
}

void loop(){

  // 3列分の光センサを計測
  for( int i=0; i<(sizeof(sensor_send_pin)/sizeof(sensor_send_pin[0])); ++i ){

    digitalWrite( sensor_send_pin[i], LOW);
    delayMicroseconds(100);
    for( int j=0; j<(sizeof(sensor_receive_pin)/sizeof(sensor_receive_pin[0])); ++j ){
      sensor_receive_val[i][j] = analogRead( sensor_receive_pin[j] );
    }
    digitalWrite( sensor_send_pin[i], HIGH);
  }

  // 光センサ計測結果を表示
  for( int i=0; i<(sizeof(sensor_receive_val)/sizeof(sensor_receive_val[0])); ++i ){
    for( int j=0; j<(sizeof(sensor_receive_val[0])/sizeof(sensor_receive_val[0][0])); ++j ){
      Serial.print( sensor_receive_val[i][j] );
      Serial.print(", ");
    }
    Serial.println("");
  }

  delay(100);
}

// 入出力ピンの設定
void init_value(){

  for( int i=0; i<(sizeof(sensor_receive_val)/sizeof(sensor_receive_val[0])); ++i ){
    for( int j=0; j<(sizeof(sensor_receive_val[0])/sizeof(sensor_receive_val[0][0])); ++j ){
      sensor_receive_val[i][j] = 0;
    }
  }

  sensor_send_pin[0] = 3;
  sensor_send_pin[1] = 5;
  sensor_send_pin[2] = 7;
  
  sensor_receive_pin[0] = A0;
  sensor_receive_pin[1] = A1;
  sensor_receive_pin[2] = A2;
  sensor_receive_pin[3] = A3;
  sensor_receive_pin[4] = A4;
  sensor_receive_pin[5] = A5;

  for( int i=0; i<(sizeof(sensor_receive_pin)/sizeof(sensor_receive_pin[0])); ++i ){
    pinMode( sensor_receive_pin[i], INPUT);
  }

  for( int i=0; i<(sizeof(sensor_send_pin)/sizeof(sensor_send_pin[0])); ++i ){
    pinMode( sensor_send_pin[i], OUTPUT);
    digitalWrite( sensor_send_pin[i], HIGH);
  }
}
