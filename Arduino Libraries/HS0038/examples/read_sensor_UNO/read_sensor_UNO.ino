#include <Wire.h>
#include <HS0038.h>
// TwoWire i2c(2, I2C_FAST_MODE);
HS0038 HS0038(&Wire, 0x09);
float angle;
void setup() {
  Serial.begin(115200);
  Wire.begin();  // Join I2C bus as master
}

void loop() {
  HS0038.read();
  Serial.print(HS0038.angle);
  Serial.print('\t');
  Serial.print(HS0038.distance);
  Serial.print('\t');
  Serial.println(HS0038.is_ball);
}
