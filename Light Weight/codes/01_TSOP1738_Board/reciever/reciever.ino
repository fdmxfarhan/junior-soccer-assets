#include <Wire.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
#define SLAVE_ADDR 0x09
#define READ_LEN 16
TwoWire i2c(2, I2C_FAST_MODE);
int mode = 0;
void setup() {
  // Serial.begin(115200);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  if(digitalRead(PA12)){
    mode ++;
    mode %= 3;
    while(digitalRead(PA12));
  }
  i2c.requestFrom(SLAVE_ADDR, READ_LEN);
  uint8_t index = 0;
  float sumX = 0, sumY = 0;
  uint8_t buffer[16];
  bool is_ball = false;
  while (i2c.available() && index < READ_LEN) {
    uint8_t data = i2c.read();
    buffer[index] = data;
    if (data > 1) {
      data = 1;
      is_ball = true;
    }
    float angleDeg = index * 22.5;
    float angleRad = angleDeg * (3.1415926 / 180.0);
    sumX += data * cos(angleRad);
    sumY += data * sin(angleRad);
    index++;
  }
  float angle = atan2(sumY, sumX) * 180.0 / 3.1415926;
  if (angle < 0) angle += 360.0;

  display.clearDisplay();
  display.setCursor(0, 0);
  if (mode == 0) {
    display.drawCircle(64, 32, 20, WHITE);
    if (is_ball) display.fillCircle(65 + 25 * sin(radians(angle)), 32 - 25 * cos(radians(angle)), 2, WHITE);
  } else if (mode == 1) {
    // display.println(buffer[0]);
    // display.println(buffer[1]);gh
    display.drawCircle(64, 32, 4, WHITE);
    for (int i = 0; i < 16; i++) {
      if (buffer[i] > 10 && buffer[i] < 1000) {
        int r = (12 + (buffer[i] / 15.0));
        display.fillCircle(65 + r * sin(radians(i * 22.5)), 32 - r * cos(radians(i * 22.5)), 2, WHITE);
      }
    }
  } else if (mode == 2){
    for(int i=0; i<8; i++){
      display.setCursor(0, i*8);
      if(i<10) display.print(' ');
      display.print(i);
      display.print(':');
      display.print(buffer[i]);
      display.setCursor(64, i*8);
      if(i+8<10) display.print(' ');
      display.print(i+8);
      display.print(':');
      display.print(buffer[i+8]);
    }
  }
  display.display();
  // Serial.print("angle: ");
  // for (uint8_t i = 0; i < index; i++) {
  // Serial.print(buffer[i]);
  // Serial.print('\t');
  // }
  // Serial.print(angle);
  // Serial.println();
}
