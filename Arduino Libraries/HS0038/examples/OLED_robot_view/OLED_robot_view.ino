#include <Wire.h>
#include <HS0038.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
HS0038 HS0038(&i2c, 0x09);
void setup() {
  // Serial.begin(115200);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  HS0038.read();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(HS0038.angle);
  display.println(HS0038.distance);
  display.drawCircle(64, 32, 20, WHITE);
  if(HS0038.is_ball){
    display.fillCircle(65 + 25 * sin(radians(HS0038.angle)), 32 - 25 * cos(radians(HS0038.angle)), 2, WHITE);
  }
  display.display();
}
