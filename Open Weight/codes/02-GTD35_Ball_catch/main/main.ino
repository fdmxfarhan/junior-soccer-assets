#include <Wire.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
int speed = 100;
int robot_x = 169;
int robot_y = 104;
float robot_angle = 0;
int ball_x;
int ball_y;
int yellow_x;
int yellow_y;
int blue_x;
int blue_y;
float ball_angle;
int ball_distance;
float yellow_angle;
int yellow_distance;
float blue_angle;
int blue_distance;
bool is_ball;
bool is_yellow;
bool is_blue;
bool use_GY = false;
int gy_sens = 1;
bool ball_in_kicker;
int Vcap = 0;
void setup() {
  Serial1.begin(115200);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
  pinMode(PB6, OUTPUT);
  pinMode(PB9, PWM);  // boost_ctrl
  digitalWrite(PB6, 1);
  init_motors();
  delay(1000);
  display_init();
  // for(int i=0; i<255; i++) Motor_SetID(i, 0);
}

void loop() {
  readGyro();
  read_MV();
  readSensors();
  Vcap = analogRead(PB0);
  if (Vcap > 650) pwmWrite(PB9, 0);
  if (Vcap < 600) pwmWrite(PB9, 3000);
  digitalWrite(PC15, digitalRead(PB4));
  // print_all();
  int bx = robot_x - ball_x;
  int by = robot_y - ball_y;
  int ba = singed_angle(ball_angle);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(bx);
  display.println(by);
  display.println(ba);
  display.println(ball_in_kicker);
  display.setCursor(64, 0);
  display.println(Vcap);
  display.display();
  if (ball_in_kicker) {
    spin(true);
    motor(0, -robot_angle, -robot_angle, 0);
  } else if (is_ball) {
    spin(true);
    moveXY(bx, by, ba);
  } else {
    motor(0, 0, 0, 0);
    spin(false);
  }
}
