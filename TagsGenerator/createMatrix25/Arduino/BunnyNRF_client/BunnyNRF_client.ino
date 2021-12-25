#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//nrf
#include <SPI.h>
#include <RH_NRF24.h>

#define DEBUG 1

//CE -> 11
//CSN -> 10
//IRW - 9

// Singleton instance of the radio driver
RH_NRF24 nrf24(9, 10);

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B);

float oriX = 0;
float oriY = 0;
float oriZ = 0;

//min, max
float maxX = -10000.0;
float minX = 100000.0;

float maxY = -10000.0;
float minY = 100000.0;

float maxZ = -10000.0;
float minZ =  100000.0;

//Pixels

//NEO PIXELS
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      32
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  pinMode(13, OUTPUT);

  
  analogWrite(A0, 255);
  delay(500)
  ;

  if (DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println("ADR 2 HIGH"); Serial.println("");
    Serial.println("Orientation Sensor Test"); Serial.println("");
  }

  bool bnoInit = bno.begin();
  /* Initialise the sensor */
  if (!bnoInit) {
    if (DEBUG)Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  } else {
    if (DEBUG) Serial.print("init!");
  }

  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);

  /* Display some basic information on this sensor */
  if (DEBUG) displaySensorDetails();

  if (!nrf24.init()) {
    if (DEBUG)Serial.println("init failed");

    
  } else {
    if (DEBUG) Serial.println("init nRF");
  }

  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)) {
    if (DEBUG)Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    if (DEBUG) Serial.println("setRF failed");
  }

  if (DEBUG) Serial.print("done setup");

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  //NEO Pixels
  pixels.begin(); // This initializes the NeoPixel library.

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  oriX = (float)event.orientation.x;
  oriY = (float)event.orientation.y;
  oriZ = (float)event.orientation.z;

  if (oriX > maxX) {
    maxX = oriX;
  }
  if (oriX < minX) {
    minX = oriX;
  }

  if (oriY > maxY) {
    maxY = oriY;
  }
  if (oriY < minY) {
    minY = oriY;
  }

  if (oriZ > maxZ) {
    maxZ = oriZ;
  }
  if (oriZ < minZ) {
    minZ = oriZ;
  }

  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  delay(BNO055_SAMPLERATE_DELAY_MS);


  if (DEBUG) {
    Serial.print(F("Orientation: "));
    Serial.print(oriX);
    Serial.print(F(" "));
    Serial.print(oriY);
    Serial.print(F(" "));
    Serial.print(oriZ);
    Serial.println(F(""));
  }
  /*
    Board layout:
    +---------- +
    |         * | RST   PITCH  ROLL  HEADING
    ADR | *        * | SCL
    INT | *        * | SDA     ^            / ->
    PS1 | *        * | GND     |            |
    PS0 | *        * | 3VO     Y    Z-- >    \ -X
    |         * | VIN
    + ---------- +
  */


  // Should be a message for us now
  // float -> 4 bytes
  uint8_t buf[3 * 4 + 4 * 1];

  float floatBuff[3];
  floatBuff[0] = oriX;
  floatBuff[1] = oriY;
  floatBuff[2] = oriZ;
  floatBuff[3] = 1.0;

  memcpy(buf, floatBuff, sizeof(floatBuff));
  nrf24.send((uint8_t*)buf, sizeof(buf));
  nrf24.waitPacketSent();
  //Serial.println( sizeof(buf));

  for (int i = 0; i < NUMPIXELS; i++) {
    int red = int(map(oriX, minX, maxX, 0, 100));
    int green = int(map(oriY, minY, maxY, 0, 155));
    int blue = int(map(oriZ, minZ, maxZ, 0, 255));

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red, green + red, blue)); // Moderately bright green color.
    pixels.show();
  }




  //The processing sketch expects data as roll, pitch, heading
  //Serial.print(F("Orientation: "));
  //Serial.print((float)event.orientation.x);
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.y);
  //Serial.print(F(" "));
  //Serial.print((float)event.orientation.z);
  //Serial.println(F(""));


  /* Also send calibration data for each sensor. */

  /*
    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    Serial.print(F("Calibration: "));
    Serial.print(sys, DEC);
    Serial.print(F(" "));
    Serial.print(gyro, DEC);
    Serial.print(F(" "));
    Serial.print(accel, DEC);
    Serial.print(F(" "));
    Serial.println(mag, DEC);
  */


}
