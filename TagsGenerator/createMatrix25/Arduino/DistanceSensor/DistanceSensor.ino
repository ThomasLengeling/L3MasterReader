void setup(){
  Serial.begin(115200);
  pinMode(5, INPUT);
}

void loop(){
  
  Serial.print(digitalRead(5), BIN);
  Serial.print(" ");
  Serial.println(analogRead(5));
  delay(100);
}
