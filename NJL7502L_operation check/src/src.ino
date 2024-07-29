void setup() {
  // put your setup code here, to run once:
  pinMode( A0, INPUT);
  pinMode( A1, INPUT);
  pinMode( 4, OUTPUT);
  pinMode( 7, OUTPUT);

  digitalWrite( 4, HIGH);
  digitalWrite( 7, HIGH);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // マトリクス的に光センサ読み取りする
  
  int tmp[4] = {0};
  digitalWrite( 4, LOW);
  // delay入れた方が動作が安定する
  // たぶんTrの反応速度分の遅れ
  delay(1);
  tmp[0] = analogRead( A0 );
  tmp[1] = analogRead( A1 );
  digitalWrite( 4, HIGH);
  
  digitalWrite( 7, LOW);
  delay(1);
  tmp[2] = analogRead( A0 );
  tmp[3] = analogRead( A1 );
  digitalWrite( 7, HIGH);

  Serial.print( tmp[0] );
  Serial.print( ", " );
  Serial.print( tmp[1] );
  Serial.print( ", " );
  Serial.print( tmp[2] );
  Serial.print( ", " );
  Serial.println( tmp[3] );

  delay(50);
}
