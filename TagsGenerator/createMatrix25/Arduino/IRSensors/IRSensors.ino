
void setup() {
  Serial.begin(9600);

}

void loop() {
  int inValue0 = analogRead(A0);
  int inValue1 = analogRead(A1);

  Serial.print(inValue0);
  Serial.print(" ");
  Serial.print(inValue1);
  Serial.println();

  delay(100);
}
