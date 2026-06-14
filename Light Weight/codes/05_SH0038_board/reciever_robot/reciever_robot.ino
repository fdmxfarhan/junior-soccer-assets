#include <Wire.h>
#include <Adafruit_SH1106_STM32.h>
#define SLAVE_ADDR 0x09
#define READ_LEN 16
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
  i2c.requestFrom(SLAVE_ADDR, READ_LEN);
  uint8_t index = 0;
  uint8_t buffer[READ_LEN];
  int maxSens = 0, maxSensIndex = 0;
  while (i2c.available() && index < READ_LEN) {
    buffer[index] = i2c.read();
    if(buffer[index] > maxSens){
      maxSens = buffer[index];
      maxSensIndex = index;
    }
    index++;
  }
  display.clearDisplay();
  // display.setCursor(0, 0);
  // display.setTextSize(3);
  // display.print(maxSensIndex);

/////////////////////////////////////////
  // for(int i=0; i<8; i++){
  //   display.setCursor(0, i*8);
  //   display.print(i);
  //   display.print(":");
  //   display.print(buffer[i]);
  // }
  // for(int i=0; i<8; i++){
  //   display.setCursor(64, i*8);
  //   display.print(i+8);
  //   display.print(":");
  //   display.print(buffer[i+8]);
  // }



/////////////////////////////////////////
  // display.drawCircle(64, 32, 20, WHITE);
  // for(int i=0; i<16; i++){
  //   float a = radians(i * 22.5);
  //   if(buffer[i] > 10) display.fillCircle(64 + sin(a)*25, 32 - cos(a)*25, 2, WHITE);
  // }



/////////////////////////////////////////
  display.drawCircle(64, 32, 20, WHITE);
  float a = radians(maxSensIndex * 22.5);
  if(maxSens > 10) display.fillCircle(64 + sin(a)*25, 32 - cos(a)*25, 2, WHITE);

  // Serial.print(angle);
  // Serial.print("\t");
  // Serial.print(dist);
  // Serial.print("\t");
  // Serial.println(is_ball);
  // delay(10);
  display.display();
}
