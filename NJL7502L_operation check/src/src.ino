void setup() {
  // put your setup code here, to run once:
  pinMode( A0, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int tmp=0;
  tmp = analogRead( A0 );

  Serial.println( tmp );

  delay(50);
}
