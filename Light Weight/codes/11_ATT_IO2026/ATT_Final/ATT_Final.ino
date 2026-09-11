#include <Wire.h>
#include <HS0038.h>  /////
#include <Adafruit_SH1106_STM32.h>
#include <TDAxis12.h>
#include <LDR.h>
#define is_forward digitalRead(PA11)

Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
HS0038 HS0038(&i2c, 0x09);  /////
LDR ldr(&i2c);

float angle_robot;
float angle_ball;

int shr;
int shl;
int shb;
int diffrent;
int last_ball_angle = 0;
int ball_stop_cnt = 0;
int aim = 0;

bool OLED_EN = false;

void setup() {
  init_motors();
  motor(0, 0, 0, 0);
  delay(500);
  init_OLED();
  i2c.begin();
}

void loop() {
  update_all();
  if (is_forward) forward();
  else goalkeeper();
}
