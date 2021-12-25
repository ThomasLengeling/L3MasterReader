int LEDPIN = 6;
int POTPin = A2;

void setup() {
  // put your setup code here, to run once:

  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int ctrAnalog = analogRead(POTPin);
  analogWrite(LEDPIN, map(ctrAnalog, 0, 1023, 0, 255));
}
