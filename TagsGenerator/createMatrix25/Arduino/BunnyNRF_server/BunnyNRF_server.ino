// nrf24_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_client
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24;

//RH_NRF24 nrf24(9, 10);
// RH_NRF24 nrf24(8, 7); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // wait for SerialUSB port to connect. Needed for Leonardo only
  if (!nrf24.init()) {
    Serial.println("init failed");
  } else {
    Serial.println("init!");
  }
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

void loop()
{

  // Should be a message for us now

  uint8_t buf[3*4 + 3*1];
  uint8_t len = sizeof(buf);
  float floatBuff[int(len / 4.0)];
  //SerialUSB.println(len);

  if (nrf24.recv(buf, &len))
  {


    memcpy(floatBuff, buf, sizeof(buf));

    Serial.print(F("Orientation: "));
    Serial.print(floatBuff[0]);
    Serial.print(F(" "));
    Serial.print(floatBuff[1]);
    Serial.print(F(" "));
    Serial.print(floatBuff[2]);
    Serial.println(F(""));

  }


}
