int pin = 3;
int control = 0;
void setup() {
  // put your setup code here, to run once:

  pinMode(3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(3, 200);
  control++;
  delay(100);
  
}
