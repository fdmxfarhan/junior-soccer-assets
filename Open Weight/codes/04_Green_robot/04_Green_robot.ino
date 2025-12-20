#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
#define MAX_SPEED 65000
#define LOW_SPEED 30000
float robot_angle;
int speed = 30000;
bool use_gyro = true;
int Vcap;
char rx_data = ' ';
int vl1=0, vl2=0, vr2=0, vr1=0;
void setup() {
  shoot_init();
  init_motors();
  oled_init();
  i2c.begin();  // Join I2C bus as master
  Serial1.begin(9600);
}

void loop() {
  update_all();
  motor(vl1, vl2, vr2, vr1);
}
