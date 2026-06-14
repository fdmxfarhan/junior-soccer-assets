#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
#include <LDR.h>
#include <libmaple/iwdg.h>
#define MAX_SPEED 65000
#define HIGH_SPEED 65000
#define MID_SPEED 50000
#define LOW_SPEED 30000
#define VERYLOW_SPEED 20000
#define Team_Yellow digitalRead(PA11) 
#define SHOOT_TO_GOAL true
#define SPIN_ON digitalWrite(PC14, 1)
#define SPIN_OFF digitalWrite(PC14, 0)
#define BALL_IN_KICKER digitalRead(PA4)
bool IS_Goaler = false;
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
LDR ldr(&i2c);
bool goal_detected = false;
bool OLED_EN = false;
bool use_gyro = true;
bool gyro_reverse = false;
bool already_shooted = false;
bool is_ball;
bool is_yellow;
bool is_blue;
bool is_goal;
bool arrived_to_goal = false;
float robot_angle;
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
float ball_distance;
int robot_x_1 = 185;  // OpenMV
int robot_y_1 = 122;  // OpenMV
int robot_x_2 = 155;  // OpenMV
int robot_y_2 = 124;  // OpenMV
int robot_x;
int robot_y;
int robot_id;
int shl, shb, shr, D;

void setup() {
  if (digitalRead(PA12)) OLED_EN = true;
  robot_id = digitalRead(PA15) + 1;
  if (robot_id == 1) {
    robot_x = robot_x_1;
    robot_y = robot_y_1;
  } else {
    robot_x = robot_x_2;
    robot_y = robot_y_2;
  }
  shoot_init();
  init_motors();
  i2c.begin();
  oled_init();
  iwdg_init(IWDG_PRE_64, 300);
  if(robot_id == 1) IS_Goaler = true;
  else IS_Goaler = false;
}

void loop() {
  update_all();
  if(IS_Goaler) defend_goal();
  else forward();
}