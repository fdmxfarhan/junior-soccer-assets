#include <Wire.h>
#include <Adafruit_SH1106_STM32.h>
#define SLAVE_ADDR 0x20
#define READ_LEN 5
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
bool is_ball = false;
float angle;
uint8_t dist;

void setup() {
  // Serial.begin(115200);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  i2c.requestFrom(0x20, READ_LEN);
  uint8_t index = 0;
  uint8_t buffer[READ_LEN];
  while (i2c.available() && index < READ_LEN) {
    buffer[index] = i2c.read();
    index++;
  }
  i2c.requestFrom(0x21, READ_LEN);
  index = 0;
  uint8_t buffer2[READ_LEN];
  while (i2c.available() && index < READ_LEN) {
    buffer2[index] = i2c.read();
    index++;
  }
  i2c.requestFrom(0x22, READ_LEN);
  index = 0;
  uint8_t buffer3[READ_LEN];
  while (i2c.available() && index < READ_LEN) {
    buffer3[index] = i2c.read();
    index++;
  }
  i2c.requestFrom(0x23, READ_LEN);
  index = 0;
  uint8_t buffer4[READ_LEN];
  while (i2c.available() && index < READ_LEN) {
    buffer4[index] = i2c.read();
    index++;
  }
  display.clearDisplay();
  for(int i=0; i<READ_LEN; i++){
    display.setCursor(0, i*8);
    display.println(buffer[i]);
  }
  for(int i=0; i<READ_LEN; i++){
    display.setCursor(30, i*8);
    display.println(buffer2[i]);
  }
  for(int i=0; i<READ_LEN; i++){
    display.setCursor(60, i*8);
    display.println(buffer3[i]);
  }
  for(int i=0; i<READ_LEN; i++){
    display.setCursor(90, i*8);
    display.println(buffer4[i]);
  }
  // for(int i=0; i<8; i++){
  //   display.setCursor(64, i*8);
  //   display.print(buffer[i+8]);
  // }
  display.display();

  // Serial.print(angle);
  // Serial.print("\t");
  // Serial.print(dist);
  // Serial.print("\t");
  // Serial.println(is_ball);
  // delay(10);
}

