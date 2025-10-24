#include <Wire.h>
#include <TSOP.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TSOP_Sensor TSOP(&i2c, 0x09);
TDAxis12 gyro(&i2c, 0x10);

float ball_angle;
float robot_angle;
int speed = 40000;
bool use_gyro = true;
void motor(int ML1, int ML2, int MR2, int MR1){
  ML1 += robot_angle*400;
  ML2 += robot_angle*400;
  MR2 += robot_angle*400;
  MR1 += robot_angle*400;

  if(ML1 > 65535) ML1 = 65535;
  if(ML1 <-65535) ML1 =-65535;
  if(ML2 > 65535) ML2 = 65535;
  if(ML2 <-65535) ML2 =-65535;
  if(MR2 > 65535) MR2 = 65535;
  if(MR2 <-65535) MR2 =-65535;
  if(MR1 > 65535) MR1 = 65535;
  if(MR1 <-65535) MR1 =-65535;
  // Motor Left 1
  if(ML1 > 0){
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  }
  else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, 65525 + ML1);
  }
  // Motor Left 2
  if(ML2 > 0){
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  }
  else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, 65525 + ML2);
  }
  // Motor Right 2
  if(MR2 > 0){
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  }
  else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, 65525 + MR2);
  }
  // Motor Right 1
  if(MR1 > 0){
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  }
  else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, 65525 + MR1);
  }
}
void moveAngle(int a) {
  if (a > 360) a -= 360;
  if (a < 0) a += 360;
  int x = speed * cos(a * M_PI / 180);
  int y = speed * sin(a * M_PI / 180);
  motor((x + y), (x - y), (-x - y), (y - x));
}
void moveXY(float Vx, float Vy, float w) {
  int speed_x = 2;
  int speed_y = 2;
  int speed_w = 2;
  int Vl1 =  Vx * speed_x + Vy * speed_y + w * speed_w;
  int Vl2 = -Vx * speed_x + Vy * speed_y + w * speed_w;
  int Vr2 = -Vx * speed_x - Vy * speed_y + w * speed_w;
  int Vr1 =  Vx * speed_x - Vy * speed_y + w * speed_w;
  motor(Vl1, Vl2, Vr2, Vr1);
}
void spin(bool state){
  digitalWrite(PC14, state);
}
void stop(){
  motor(0, 0, 0, 0);
}
void setup() {
  // Serial.begin(115200);
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);
  pinMode(PB8, PWM);
  pinMode(PB9, PWM);
  motor(0, 0, 0, 0);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
  pinMode(PA8, PWM);
  pwmWrite(PA8, 3000);
}

void loop() {
  ball_angle = TSOP.read();
  robot_angle = gyro.read();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawCircle(64, 32, 20, WHITE);
  if(TSOP.is_ball) display.fillCircle(65 + 25 * sin(radians(ball_angle)), 32 - 25 * cos(radians(ball_angle)), 2, WHITE);
  display.drawLine(65 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 65 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  display.display();
  if(TSOP.is_ball){
    moveAngle(ball_angle);
    // if(ball_angle < 20 && ball_angle > 340) moveAngle(ball_angle);
    // else if(ball_angle < 180) moveAngle(ball_angle + 90);
    // else moveAngle(ball_angle - 90);
  }
  else{
    stop();
  }
}
