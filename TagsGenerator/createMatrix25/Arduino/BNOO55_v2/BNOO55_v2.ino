#include <i2c_t3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055_t3.h>
#include <utility/imumaths.h>
#include <Encoder.h>

Adafruit_BNO055 bno  = Adafruit_BNO055(WIRE_BUS, -1, BNO055_ADDRESS_A, I2C_MASTER, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_100, I2C_OP_MODE_ISR);
Adafruit_BNO055 bnoTwo = Adafruit_BNO055(WIRE1_BUS, 1, BNO055_ADDRESS_A, I2C_MASTER, I2C_PINS_29_30, I2C_PULLUP_INT, I2C_RATE_100, I2C_OP_MODE_ISR);

void setup() {
   
  Serial.begin(115200);
  if(!bno.begin()){    
    while(1);
    Serial.println("bno not starting up");
  }else{
    Serial.println("bno seems to be working!");
  }
  
  if(!bnoTwo.begin()){    
    while(1);
    Serial.println("bnoTwo not starting up");
  }else{
    Serial.println("bnoTwo seems to be working!");
  }

  delay(1000);
  
  //Use crystal for extra precision  
  bno.setExtCrystalUse(true);
  bnoTwo.setExtCrystalUse(true);

}


void loop(void) {
   
    //get dof data
    sensors_event_t event;
    bno.getEvent(&event);
    bnoTwo.getEvent(&event);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imu::Vector<3> eulerTwo = bnoTwo.getVector(Adafruit_BNO055::VECTOR_EULER);
    
    Serial.print(kopValue); Serial.print(",");
    Serial.print(euler.x()); Serial.print(",");
    Serial.print(euler.y()); Serial.print(",");
    Serial.print(euler.z()); Serial.print(",");
    Serial.print(eulerTwo.x()); Serial.print(",");
    Serial.print(eulerTwo.y()); Serial.print(",");
    Serial.println(eulerTwo.z());
    }      
}
