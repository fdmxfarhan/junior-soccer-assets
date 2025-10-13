#include <Wire.h>

#define SLAVE_ADDR 0x09
#define READ_LEN 16

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Join I2C bus as master
}

void loop() {
  Wire.requestFrom(SLAVE_ADDR, READ_LEN);
  uint8_t index = 0;
  float sumX = 0, sumY = 0;
  uint8_t buffer[16];
  while (Wire.available() && index < READ_LEN) {
    uint8_t data = Wire.read();
    buffer[index] = data;
    float angleDeg = index * 22.5;
    float angleRad = angleDeg * (3.1415926 / 180.0);
    sumX += data * cos(angleRad);
    sumY += data * sin(angleRad);
    index++;
  }
  float angle = atan2(sumY, sumX) * 180.0 / 3.1415926;
  if (angle < 0) angle += 360.0;

  // Serial.print("angle: ");
  for (uint8_t i = 0; i < index; i++) {
    Serial.print(buffer[i]);
    Serial.print('\t');
  }
  Serial.print(angle);
  Serial.println();
  delay(100);
}
