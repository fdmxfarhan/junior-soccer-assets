#include <Wire.h>
#include <HS0038.h>
#include <TDAxis12.h>
#include <LDR.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
HS0038 HS0038(&i2c, 0x09);
TDAxis12 gyro(&i2c, 0x10);
LDR LDR(&i2c);
#define MAX_SPEED 65000
float robot_angle;
int speed = 55000;
bool use_gyro = true;
int shl, shr, shb, dif;
void motor(int ML1, int ML2, int MR2, int MR1) {
  ML1 += robot_angle * 400;
  ML2 += robot_angle * 400;
  MR2 += robot_angle * 400;
  MR1 += robot_angle * 400;

  ML1 = -ML1;
  ML2 = -ML2;
  MR2 = -MR2;
  MR1 = -MR1;

  if (ML1 > MAX_SPEED) ML1 = MAX_SPEED;
  if (ML1 < -MAX_SPEED) ML1 = -MAX_SPEED;
  if (ML2 > MAX_SPEED) ML2 = MAX_SPEED;
  if (ML2 < -MAX_SPEED) ML2 = -MAX_SPEED;
  if (MR2 > MAX_SPEED) MR2 = MAX_SPEED;
  if (MR2 < -MAX_SPEED) MR2 = -MAX_SPEED;
  if (MR1 > MAX_SPEED) MR1 = MAX_SPEED;
  if (MR1 < -MAX_SPEED) MR1 = -MAX_SPEED;
  // Motor Left 1
  if (ML1 > 0) {
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  } else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, 65525 + ML1);
  }
  // Motor Left 2
  if (ML2 > 0) {
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  } else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, 65525 + ML2);
  }
  // Motor Right 2
  if (MR2 > 0) {
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  } else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, 65525 + MR2);
  }
  // Motor Right 1
  if (MR1 > 0) {
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  } else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, 65525 + MR1);
  }
}
void moveAngle(int a, int v) {
  if (a > 360) a -= 360;
  if (a < 0) a += 360;
  int x = v * cos(a * M_PI / 180);
  int y = v * sin(a * M_PI / 180);
  motor((x + y), (x - y), (-x - y), (y - x));
}
void moveInside(){
  if(LDR.front) moveAngle(180, 30000);
  else if(LDR.back) moveAngle(0, 30000);
  else if(LDR.right) moveAngle(270, 30000);
  else if(LDR.left) moveAngle(90, 30000);
  else stop();
}
void moveForSec(int a, int duration) {
  for (int i = 0; i < duration; i++) {
    moveAngle(a, 60000);
    update_all();
  }
}
void moveXY(float Vx, float Vy, float w) {
  int speed_x = 2;
  int speed_y = 2;
  int speed_w = 2;
  int Vl1 = Vx * speed_x + Vy * speed_y + w * speed_w;
  int Vl2 = -Vx * speed_x + Vy * speed_y + w * speed_w;
  int Vr2 = -Vx * speed_x - Vy * speed_y + w * speed_w;
  int Vr1 = Vx * speed_x - Vy * speed_y + w * speed_w;
  motor(Vl1, Vl2, Vr2, Vr1);
}
void spin(bool state) {
  digitalWrite(PC14, state);
}
void stop() {
  motor(0, 0, 0, 0);
}
void update_all() {
  robot_angle = gyro.read();
  HS0038.read();
  LDR.read();
  shl = analogRead(PA0);
  shb = analogRead(PA1);
  shr = analogRead(PA2);
  dif = shr - shl;
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("D: ");
  display.println(HS0038.distance);
  display.println(shl);
  display.println(shb);
  display.println(shr);
  display.drawCircle(64, 32, 20, WHITE);
  if (HS0038.is_ball) display.fillCircle(65 + 25 * sin(radians(HS0038.angle)), 32 - 25 * cos(radians(HS0038.angle)), 2, WHITE);
  if (LDR.right) display.fillRoundRect(70, 30, 10, 4, 2, WHITE);
  if (LDR.left) display.fillRoundRect(48, 30, 10, 4, 2, WHITE);
  if (LDR.front) display.fillRoundRect(62, 16, 4, 10, 2, WHITE);
  if (LDR.back) display.fillRoundRect(62, 40, 4, 10, 2, WHITE);
  display.drawLine(65 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 65 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  display.display();
}
void out() {
  if (LDR.left) {
    moveForSec(90, 9);
    float out_ball_angle = HS0038.angle;
    while (HS0038.angle > 190 && HS0038.angle < 350 && HS0038.is_ball) {
      update_all();
      moveInside();
    }
  }
  if (LDR.right) {
    moveForSec(270, 9);
    float out_ball_angle = HS0038.angle;
    while (HS0038.angle > 10 && HS0038.angle < 170 && HS0038.is_ball) {
      update_all();
      moveInside();
    }
  }
  if (LDR.front) {
    moveForSec(180, 9);
    float out_ball_angle = HS0038.angle;
    while ((HS0038.angle > 270 || HS0038.angle < 90) && HS0038.is_ball) {
      update_all();
      moveInside();
    }
  }
  if (LDR.back) {
    moveForSec(0, 9);
    float out_ball_angle = HS0038.angle;
    while ((HS0038.angle > 90 && HS0038.angle < 270) && HS0038.is_ball) {
      update_all();
      moveInside();
    }
  }
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
  pinMode(PA3, INPUT_PULLUP);
}
void loop() {
  update_all();
  out();
  if(!digitalRead(PA3)){
    moveAngle(0, MAX_SPEED);
  }
  else if (HS0038.is_ball) {
    // moveAngle(HS0038.angle);
    if (HS0038.distance < 30) moveAngle(HS0038.angle, 50000);
    else if (HS0038.angle < 25 || HS0038.angle > 335) moveAngle(HS0038.angle, 40000);
    else if (HS0038.angle < 50) moveAngle(HS0038.angle + 20, 40000);
    else if (HS0038.angle < 180) moveAngle(HS0038.angle + 70, 40000);
    else if (HS0038.angle < 310) moveAngle(HS0038.angle - 70, 40000);
    else moveAngle(HS0038.angle - 20, 40000);
  } else {
    if(shb < 1000) motor(dif * 10 - 30000, -dif * 10 - 30000, -dif * 10 + 30000, dif * 10 + 30000);
    else motor(dif * 10, -dif * 10, -dif * 10, dif * 10);
  }
}
