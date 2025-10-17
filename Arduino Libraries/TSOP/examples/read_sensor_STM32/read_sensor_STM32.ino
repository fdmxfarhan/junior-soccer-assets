#include <Wire.h>
#include <TSOP.h>
TwoWire i2c(2, I2C_FAST_MODE);
TSOP_Sensor TSOP(&i2c, 0x09);
float angle;
void setup() {
  i2c.begin();  // Join I2C bus as master
  Serial.begin(9600);
}

void loop() {
  angle = TSOP.read();
  if(TSOP.is_ball) Serial.println(angle);
}
