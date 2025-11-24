#include <Wire.h>
#include <LDR.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
LDR LDR(&i2c);
void setup() {
  // Serial.begin(115200);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  LDR.read();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Front: ");
  display.println(LDR.front);
  display.print("Rgiht: ");
  display.println(LDR.right);
  display.print("Back: ");
  display.println(LDR.back);
  display.print("Left: ");
  display.println(LDR.left);
  display.display();
}
