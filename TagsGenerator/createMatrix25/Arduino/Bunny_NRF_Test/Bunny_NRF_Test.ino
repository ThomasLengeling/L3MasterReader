//nrf
#include <SPI.h>
#include <RH_NRF24.h>

#define DEBUG 1

//CE -> 11
//CSN -> 10
//IRW - 9

// Singleton instance of the radio driver
RH_NRF24 nrf24(9, 10);



int counterY = 0;
float addedY = 0.0;


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{

  if (!nrf24.init()) {
   // if (DEBUG)Serial.println("init failed");
  } else {
  //  if (DEBUG) Serial.println("init nRF");
  }

  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)) {
  //  if (DEBUG)Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
   // if (DEBUG) Serial.println("setRF failed");
  }

 // if (DEBUG) Serial.print("done setup");



}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{

  // Should be a message for us now
  // float -> 4 bytes
  uint8_t buf[3 * 4 + 4 * 1];

  float floatBuff[3];
  floatBuff[0] = 0.5;
  floatBuff[1] = 0.5 + addedY;
  floatBuff[2] = 1.0;
  floatBuff[3] = 1.0;

  memcpy(buf, floatBuff, sizeof(floatBuff));
  nrf24.send((uint8_t*)buf, sizeof(buf));
  nrf24.waitPacketSent();
  //Serial.println( sizeof(buf));

  if (counterY > 500) {
    addedY += 0.01;
    if (addedY > 0.5) {
      addedY = 0;
    }
    counterY = 0;
  }
  counterY++;

}
