#include <Wire.h>
#include <TSOP.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TSOP_Sensor TSOP(&i2c, 0x09);
float angle;
void setup() {
  // Serial.begin(115200);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  angle = TSOP.read();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawCircle(64, 32, 20, WHITE);
  display.fillCircle(65 + 25 * sin(radians(angle)), 32 - 25 * cos(radians(angle)), 2, WHITE);
  display.display();
}
