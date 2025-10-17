#include <Wire.h>
#include <TSOP.h>
TSOP_Sensor TSOP(&Wire, 0x09);
float angle;
void setup() {
  Wire.begin();  // Join I2C bus as master
  Serial.begin(9600);
}

void loop() {
  angle = TSOP.read();
  if(TSOP.is_ball) Serial.println(angle);
}
