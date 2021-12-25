#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

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

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B);

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
  SerialUSB.println("------------------------------------");
  SerialUSB.print  ("Sensor:       "); SerialUSB.println(sensor.name);
  SerialUSB.print  ("Driver Ver:   "); SerialUSB.println(sensor.version);
  SerialUSB.print  ("Unique ID:    "); SerialUSB.println(sensor.sensor_id);
  SerialUSB.print  ("Max Value:    "); SerialUSB.print(sensor.max_value); SerialUSB.println(" xxx");
  SerialUSB.print  ("Min Value:    "); SerialUSB.print(sensor.min_value); SerialUSB.println(" xxx");
  SerialUSB.print  ("Resolution:   "); SerialUSB.print(sensor.resolution); SerialUSB.println(" xxx");
  SerialUSB.println("------------------------------------");
  SerialUSB.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  SerialUSB.begin(115200);

  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }



  SerialUSB.println("ADR 2 HIGH"); SerialUSB.println("");


  SerialUSB.println("Orientation Sensor Test"); SerialUSB.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    SerialUSB.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  } else {
    SerialUSB.print("init!");
  }

  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);

  /* Display some basic information on this sensor */
  displaySensorDetails();


  Serial.println("Please Calibrate Sensor: ");
  while (!bno.isFullyCalibrated())
  {
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
  }

  SerialUSB.print("done setup");
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

  /* The processing sketch expects data as roll, pitch, heading */
  SerialUSB.print(F("Orientation: "));
  SerialUSB.print((float)event.orientation.x);
  SerialUSB.print(F(" "));
  SerialUSB.print((float)event.orientation.y);
  SerialUSB.print(F(" "));
  SerialUSB.print((float)event.orientation.z);
  SerialUSB.println(F(""));

  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  SerialUSB.print(F("Calibration: "));
  SerialUSB.print(sys, DEC);
  SerialUSB.print(F(" "));
  SerialUSB.print(gyro, DEC);
  SerialUSB.print(F(" "));
  SerialUSB.print(accel, DEC);
  SerialUSB.print(F(" "));
  SerialUSB.println(mag, DEC);

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
