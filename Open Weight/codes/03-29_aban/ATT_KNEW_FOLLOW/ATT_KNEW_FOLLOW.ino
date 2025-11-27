#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
#include <PixyI2C.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gy(&i2c, 0x10);
PixyI2C pixy;

int speed = 390;
float ball_x;
float ball_y;
float ball_x_pixy;
float ball_y_pixy;
float v = 2.5;
float v_pixy = 1;
float v_gardesh = 1.59;
float v_gardesh_pixy = 1;
bool TDAxis_enable = false;
bool is_yellow;
int primery_speed = 300;
float robot_angle;
float yellow_angle;
float yellow_x;
float yellow_y;
///////////////////////////////////////////// ROBOT 1
int robot_x_1 = 172;  // OpenMV
int robot_y_1 = 113;  // OpenMV
int robot_x_pixy_1 = 148;  
int robot_y_pixy_1 = 250;
///////////////////////////////////////////// ROBOT 2
int robot_x_2 = 159;  // OpenMV
int robot_y_2 = 100;  // OpenMV
int robot_x_pixy_2 = 148;  
int robot_y_pixy_2 = 250;

int robot_x;  ////172
int robot_y;
int robot_x_pixy;  ////172
int robot_y_pixy;
float ball_angle;
float ball_angle_pixy;
float ball_distance_meter;
bool is_ball;
bool is_ball_pixy;
int a = 100;
int b = 90;
int mode = 0;
int shl, shb, shr, d;
uint8_t robot_id;
float turn(int main_rpm, int moment_rpm) {
  float left_right_angle = (60 / (moment_rpm / main_rpm));
}
void setup() {
  Serial1.begin(115200);
  i2c.begin();
  display.begin(0x2, 0x3c);
  pinMode(PA3, INPUT);    //////IR
  pinMode(PC14, OUTPUT);  ///////spiner
  pinMode(PC15, OUTPUT);  ////SHOOT_RELAY
  pinMode(PB9, PWM);      /////SHOOT_Mosfet
  robot_id = digitalRead(PA15) + 1;
  if(robot_id == 1){
    robot_x = robot_x_1;
    robot_y = robot_y_1;
    robot_x_pixy = robot_x_pixy_1;
    robot_y_pixy = robot_y_pixy_1;
  }else{
    robot_x = robot_x_2;
    robot_y = robot_y_2;
    robot_x_pixy = robot_x_pixy_2;
    robot_y_pixy = robot_y_pixy_2;
  }
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("ID:");
  display.print(robot_id);
  display.display();
  delay(900);
  display.setTextSize(1);
  pixy.init();
}

void loop() {
  robot_angle = gy.read();
  print_all();
  read_MV();
  if(robot_id == 1)read_pixy();
  sensor();
  shoot_init(2500);

  if (digitalRead(PA12) == 1) {
    Rotate_Move_AI();
  } else {
    No_Rotate_AI();
  }
}