int pinOut = 13;

void setup() {
  // put your setup code here, to run once:

  SerialUSB.begin(9600);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(13, INPUT);

  
  pinMode(9, INPUT);
 // digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  int val = analogRead(9);
  SerialUSB.println(val);

}
