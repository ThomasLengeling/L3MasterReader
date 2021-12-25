#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Adafruit_NeoPixel.h>

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

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


//NEO PIXELS
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      32

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B);

float orientationX = 0;
float orientationY = 0;
float orientationZ = 0;

float maxX = -10000.0;
float minX = 100000.0;

float maxY = -10000.0;
float minY = 100000.0;

float maxZ = -10000.0;
float minZ =  100000.0;


// x[0, 360]
// y[-86, 86]
// z[-180, 180]

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
  Serial.begin(115200);

  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}



  Serial.println("ADR 2 HIGH"); Serial.println("");


  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  } else {
    Serial.print("init!");
  }

  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);

  /* Display some basic information on this sensor */
  displaySensorDetails();


  sensors_event_t event;
  bno.getEvent(&event);
  Serial.println("Please Calibrate Sensor: ");
  //while (!bno.isFullyCalibrated())
  //{
  bno.getEvent(&event);

  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);

  /* Optional: Display calibration status */
  displayCalStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting new data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
  // }

  Serial.print("done setup");

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

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */


  orientationX = (float)event.orientation.x;
  orientationY = (float)event.orientation.y;
  orientationZ = (float)event.orientation.z;

  /* The processing sketch expects data as roll, pitch, heading */
  Serial.print(F("Orientation: "));
  Serial.print(orientationX);
  Serial.print(F(" "));
  Serial.print(orientationY);
  Serial.print(F(" "));
  Serial.print(orientationZ);
  Serial.println(F(""));

  if (orientationX > maxX) {
    maxX = orientationX;
  }
  if (orientationX < minX) {
    minX = orientationX;
  }

  if (orientationY > maxY) {
    maxY = orientationY;
  }
  if (orientationY < minY) {
    minY = orientationY;
  }

  if (orientationZ > maxZ) {
    maxZ = orientationZ;
  }
  if (orientationZ < minZ) {
    minZ = orientationZ;
  }

  for (int i = 0; i < NUMPIXELS; i++) {
    int red = int(map(orientationX, minX, maxX, 0, 255));
    int green = int(map(orientationY, minY, maxY, 0, 255));
    int blue = int(map(orientationZ, minZ, maxZ, 0, 255));

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
    pixels.show();
  }

  ble.write(x);


  Serial.print(F("m X: "));
  Serial.print(minX);
  Serial.print(" ");
  Serial.print(maxX);
  Serial.print(F(" m Y: "));
  Serial.print(minY);
  Serial.print(" ");
  Serial.print(maxY);
  Serial.print(F(" m Z: "));
  Serial.print(minZ);
  Serial.print(" ");
  Serial.println(maxZ);

  /* Also send calibration data for each sensor. */
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

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}
