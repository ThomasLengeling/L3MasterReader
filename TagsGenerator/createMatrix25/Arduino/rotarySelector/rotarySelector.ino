#include <Adafruit_NeoPixel.h>
#include <Encoder.h>

Encoder myEnc(5, 6);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 8, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;

    for(int i = 0 ; i < 8; i++){
      strip.setPixelColor(i, 0);
    }

    int dot = abs(newPosition)%8;
    strip.setPixelColor(dot, Wheel((newPosition*5) & 255));
    strip.show();

  }

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
