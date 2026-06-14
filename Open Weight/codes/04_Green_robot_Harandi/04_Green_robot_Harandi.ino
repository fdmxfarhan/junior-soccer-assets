#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
#include <PixyI2C.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
#define MAX_SPEED 65000
#define LOW_SPEED 30000
#define Team_Yellow true
#define Ball_In_Kicker digitalRead(PA3)
PixyI2C pixy;
float robot_angle;
int speed = 30000;
bool use_gyro = true;
int Vcap;
char rx_data = ' ';
int vl1 = 0, vl2 = 0, vr2 = 0, vr1 = 0;
float ball_x;
float ball_y;
float ball_distance;
float yellow_angle;
float yellow_x;
float yellow_y;
float blue_angle;
float blue_x;
float blue_y;
float goal_angle;
float goal_x;
float goal_y;
float ball_angle;
bool is_ball;
bool is_yellow;
bool is_blue;
bool is_goal;
int robot_x = 160;
int robot_y = 112;
bool already_shooted = false;
float shift = 0;
int kick_timer = 0;
bool use_gy = false;
void setup() {
  shoot_init();
  init_motors();
  oled_init();
  i2c.begin();  // Join I2C bus as master
  Serial1.begin(9600);
  pixy.init();
}

void loop() {
  update_all();
  if (Ball_In_Kicker) {
    use_gy = true;
    if (kick_timer < 50) {
      digitalWrite(PC14, 1);
      stop();
    } else {
      shoot();
      moveAngle(0, LOW_SPEED);
    }
    kick_timer++;
  } else if (is_ball) {
    digitalWrite(PC14, 1);
    moveXY(ball_y - robot_y, robot_x - ball_x, ball_angle);
    //moveAngle(ball_angle + shift, MAX_SPEED);
  } else {
    digitalWrite(PC14, 0);
    stop();
  }
}
