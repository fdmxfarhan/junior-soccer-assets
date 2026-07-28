#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
#include <PixyI2C.h>
#include <LDR.h>
#include <libmaple/iwdg.h>
#define BALL_IN_KICKER !digitalRead(PA3)
#define Team_Yellow digitalRead(PA11)
#define VERYLOW_SPEED 20
#define LOW_SPEED 90
#define MID_SPEED 200
#define HIGH_SPEED 400
#define SHOOT_TO_GOAL true
#define SPIN_ON digitalWrite(PC14, 1)
#define SPIN_OFF digitalWrite(PC14, 0)
#define OUT_OF_SIGHT_ROBOT_1 50
#define OUT_OF_SIGHT_ROBOT_2 50
#define IS_Goaller !digitalRead(PA12)

Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gy(&i2c, 0x10);
PixyI2C pixy;
LDR ldr(&i2c);
int speed = 390;
float ball_x;
float ball_y;
float ball_x_pixy;
float ball_y_pixy;
float ball_distance;
float ball_distance_pixy;
int OUT_OF_SIGHT;
//┬┴┬┴┤(･_├┬┴┬┴

float v = 5;
float v_gardesh = 3;

//^_____^

float v_pixy = 2;
float v_gardesh_pixy = 1.0;


bool TDAxis_enable = false;
bool TDAxis_reverse = false;
bool is_yellow = false;
bool is_blue = false;
bool is_goal = false;
bool is_yellow_pixy = false;
bool is_blue_pixy = false;
bool is_goal_pixy = false;
bool already_shooted = false;
bool arrived_to_goal = false;
bool goal_detected = false;
int primery_speed = 300;
float robot_angle;
float yellow_angle;
float yellow_x;
float yellow_y;
float blue_angle;
float blue_x;
float blue_y;
float goal_angle;
float goal_x;
float goal_y;
float yellow_angle_pixy;
float yellow_x_pixy;
float yellow_y_pixy;
float blue_angle_pixy;
float blue_x_pixy;
float blue_y_pixy;
float goal_angle_pixy;
float goal_x_pixy;
float goal_y_pixy;
///////////////////////////////////////////// ROBOT 1
int robot_x_1 = 163;  // OpenMV
int robot_y_1 = 130;  // OpenMV
int robot_x_pixy_1 = 156;
int robot_y_pixy_1 = 250;
///////////////////////////////////////////// ROBOT 2
int robot_x_2 = 170;  // OpenMV
int robot_y_2 = 117;  // OpenMV
int robot_x_pixy_2 = 153;
int robot_y_pixy_2 = 250;

int robot_x;  ////172
int robot_y;
int robot_x_pixy;  ////172
int robot_y_pixy;
float ball_angle;
float ball_angle_pixy;
float ball_distance_meter;
bool is_ball = false;
bool is_ball_pixy = false;
int a = 100;
int b = 90;
int mode = 0;
int shl, shb, shr, d;
uint8_t robot_id;
int cap;
int BALL_IN_KICKER_Timer = 0;
bool OLED_EN = false;

void setup() {
  if (digitalRead(PB4)) OLED_EN = true;
  pinMode(PA8, OUTPUT);
  digitalWrite(PA8, 0);
  Serial1.begin(115200);
  i2c.begin();
  display.begin(0x3c);
  pinMode(PA3, INPUT);    //////IR
  pinMode(PC14, OUTPUT);  ///////spiner
  pinMode(PC15, OUTPUT);  ////SHOOT_RELAY
  pinMode(PB15, OUTPUT);  ////SHOOT_RELAY
  pinMode(PB9, PWM);      /////SHOOT_Mosfet
  shoot_init(0);
  robot_id = digitalRead(PA15) + 1;
  if (robot_id == 1) {
    robot_x = robot_x_1;
    robot_y = robot_y_1;
    robot_x_pixy = robot_x_pixy_1;
    robot_y_pixy = robot_y_pixy_1;
    OUT_OF_SIGHT = OUT_OF_SIGHT_ROBOT_1;
  } else {
    robot_x = robot_x_2;
    robot_y = robot_y_2;
    robot_x_pixy = robot_x_pixy_2;
    robot_y_pixy = robot_y_pixy_2;
    OUT_OF_SIGHT = OUT_OF_SIGHT_ROBOT_2;
  }
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("ID:");
  display.println(robot_id);
  if (digitalRead(PA12)) display.println("Forward");
  else display.println("Goaler");
  if (digitalRead(PA11)) display.println("Yellow");
  else display.println("Blue");
  display.display();
  delay(900);
  display.setTextSize(1);
  pixy.init();
  iwdg_init(IWDG_PRE_64, 300);
}

void loop() {
  update_all();
  if (IS_Goaller) {
    GoalKeeper();
  } else {
    Rotate_Move_AI();
  }
}