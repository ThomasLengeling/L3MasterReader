void setup() {
  // put your setup code here, to run once:

  pinMode(12, INPUT);
  pinMode(13, OUTPUT);

  SerialUSB.begin(9600);
  while (!SerialUSB)
    ;
}

void loop() {
  // put your main code here, to run repeatedly:

  int heffect = digitalRead(12);
  delay(50);

  SerialUSB.println(heffect);
  if (heffect == HIGH) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

}
