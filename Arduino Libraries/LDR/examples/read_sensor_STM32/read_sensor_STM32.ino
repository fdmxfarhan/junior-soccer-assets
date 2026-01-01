#include <Wire.h>
#include <HS0038.h>
TwoWire i2c(2, I2C_FAST_MODE);
HS0038 LDR(&i2c, 0x09);
float angle;
void setup() {
  Serial.begin(115200);
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  LDR.read();
  Serial.print(LDR.front);
  Serial.print('\t');
  Serial.print(LDR.right);
  Serial.print('\t');
  Serial.print(LDR.back);
  Serial.print('\t');
  Serial.println(LDR.left);
}
