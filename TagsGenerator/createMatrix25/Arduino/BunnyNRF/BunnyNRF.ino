#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//nrf
#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24;

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

  if (!nrf24.init()) {
    SerialUSB.println("init failed");
  } else {
    SerialUSB.println("init nRF");
  }

  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)) {
    SerialUSB.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    SerialUSB.println("setRF failed");
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

  oriX = (float)event.orientation.x;
  oriY = (float)event.orientation.y;
  oriZ = (float)event.orientation.;

  SerialUSB.print(F("Orientation: "));
  SerialUSB.print(oriX);
  SerialUSB.print(F(" "));
  SerialUSB.print(oriY);
  SerialUSB.print(F(" "));
  SerialUSB.print(oriZ);
  SerialUSB.println(F(""));


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

  if (nrf24.available())
  {
    // Should be a message for us now
    float buf[3];
    float len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      //      NRF24::printBuffer("request: ", buf, len);
      SerialUSB.print("got request: ");
      SerialUSB.println((float*)buf);

      data[0] = oriX;
      data[1] = oriY;
      data[2] = oriZ;

      // Send a reply
      uint8_t data[] = "And hello back to you";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      //SerialUSB.println("Sent a reply");
    }
    else
    {
      SerialUSB.println("recv failed");
    }
  }


  /* The processing sketch expects data as roll, pitch, heading
    SerialUSB.print(F("Orientation: "));
    SerialUSB.print((float)event.orientation.x);
    SerialUSB.print(F(" "));
    SerialUSB.print((float)event.orientation.y);
    SerialUSB.print(F(" "));
    SerialUSB.print((float)event.orientation.z);
    SerialUSB.println(F(""));
  */

  /* Also send calibration data for each sensor. */

  /*
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
  */

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
