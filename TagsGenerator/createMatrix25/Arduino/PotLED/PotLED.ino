int pinBrightness = A0;
int pinStrobes = A1;

int ledPin = 13;

int strobeTimer = 0;
bool strobeActive = false;
bool stobeLock = false;

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);
}

void loop() {

  int cTime = millis();

  int analogBrigtness = analogRead(A0);
  int analogStrobe   = analogRead(A1);

  int brightness = int(map(analogBrigtness, 0, 1023, 0, 255));
  int timerThreshold = int(map(analogStrobe, 0, 1023, 0, 2000));

   analogWrite(ledPin, brightness);
/*
  if (!stobeLock) {
    if (abs(cTime - strobeTimer) > timerThreshold) {
      strobeTimer = cTime;
      strobeActive = !strobeActive;
    }
  }

  if (strobeActive) {
    analogWrite(ledPin, brightness);
  } else {
    analogWrite(ledPin, 0);
  }
  */
}
