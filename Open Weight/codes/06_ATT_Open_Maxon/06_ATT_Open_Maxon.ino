#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
#define MAX_SPEED 65000
#define HIGH_SPEED 65000
#define MID_SPEED 50000
#define LOW_SPEED 30000
#define Team_Yellow true
#define SPIN_ON digitalWrite(PC14, 1)
#define SPIN_OFF digitalWrite(PC14, 0)
#define BALL_IN_KICKER digitalRead(PA4)
float robot_angle;
int speed = 30000;
bool use_gyro = true;
bool already_shooted = false;
int Vcap;
char rx_data = ' ';
float ball_x;
float ball_y;
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
int robot_x = 146;
int robot_y = 134;

void setup() {
  shoot_init();
  init_motors();
  oled_init();
  i2c.begin(); 
}

void loop() {
  update_all();
  if(BALL_IN_KICKER){
    move(0, HIGH_SPEED);
    shoot();
  }
  if(is_ball){
    float shift = clamp(ball_angle * 0.9, -90, 90);
    move(ball_angle + shift, HIGH_SPEED);
  }
  else stop();
}
