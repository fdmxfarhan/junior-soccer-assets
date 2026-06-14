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
#define Team_Yellow true
#define SPIN_ON digitalWrite(PC14, 1)
#define SPIN_OFF digitalWrite(PC14, 0)
#define BALL_IN_KICKER digitalRead(PA4)
#define IS_Goaler !digitalRead(PA11)

Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
LDR ldr(&i2c);

bool OLED_EN = false;
bool use_gyro = true;
bool gyro_reverse = false;
bool already_shooted = false;
bool is_ball, is_yellow, is_blue, is_goal;
bool arrived_to_goal = false;
float robot_angle;
float ball_x, ball_y, ball_angle, ball_distance;
float yellow_x, yellow_y, yellow_angle;
float blue_x, blue_y, blue_angle;
float goal_x, goal_y, goal_angle;
///////////////////////////////////////////// ROBOT 1
int robot_x_1 = 171;  // OpenMV
int robot_y_1 = 118;  // OpenMV
///////////////////////////////////////////// ROBOT 2
int robot_x_2 = 146;  // OpenMV
int robot_y_2 = 134;  // OpenMV
int robot_x, robot_y;
int robot_id;
int shl, shb, shr, D;
enum RobotState {
  STATE_IDLE,
  STATE_SEARCH_BALL,
  STATE_CATCH_BALL,
  STATE_GOAL,
  STATE_DEFEND,
  STATE_OUT
};

RobotState state = STATE_IDLE;

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
  oled_init();
  i2c.begin();
  iwdg_init(IWDG_PRE_64, 600);  // ≈ 2 seconds
}

void loop() {
  update_all();
  if (IS_Goaler) defend_goal();
  else forward();
}

// void loop() {
//   update_all();
//   update_state();

//   switch (state) {
//     case STATE_IDLE:
//       stop();
//       break;

//     case STATE_SEARCH_BALL:
//       come_back2();
//       break;

//     case STATE_CATCH_BALL:
//       catch_ball();
//       break;

//     case STATE_GOAL:
//       goal();
//       break;

//     case STATE_DEFEND:
//       defend_goal();
//       break;

//     case STATE_OUT:
//       out();
//       break;
//   }
// }
