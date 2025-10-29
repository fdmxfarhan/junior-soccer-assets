#include <Wire.h>
#define SLAVE_ADDR 0x09
#define READ_LEN 3
bool is_ball = false;
float angle;
uint8_t dist;

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Join I2C bus as master
}

void loop() {
  Wire.requestFrom(SLAVE_ADDR, READ_LEN);
  uint8_t index = 0;
  uint8_t buffer[3];
  while (Wire.available() && index < READ_LEN) {
    buffer[index] = Wire.read();;
    index++;
  }
  if(buffer[0] == 0xFF && buffer[1] == 0xFF && buffer[2] == 0xFF) is_ball = false;
  else {
    is_ball = true;
    angle = ((buffer[0] << 8) | buffer[1])/100.0;
    dist = buffer[2];
  }
  Serial.print(angle);
  Serial.print("\t");
  Serial.print(dist);
  Serial.print("\t");
  Serial.println(is_ball);
  delay(10);
}
