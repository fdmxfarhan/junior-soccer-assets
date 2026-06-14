#include <Wire.h>
#include <HS0038.h>  /////
#include <Adafruit_SH1106_STM32.h>
#include <TDAxis12.h>
#include <LDR.h>
int shr;
int shl;
int shb;
int diffrent;
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gyro(&i2c, 0x10);
HS0038 HS0038(&i2c, 0x09);  /////
LDR ldr(&i2c);
float angle_robot;
float angle_ball;
int v = (45000);
bool cheking_out = false;
#define OUT_SENS 3
bool already_kicked = false;
void motor(int ML1, int ML2, int MR2, int MR1) {
  ML1 += (angle_robot * 300);
  ML2 += (angle_robot * 300);
  MR2 += (angle_robot * 300);
  MR1 += (angle_robot * 300);
  if (ML1 > 65535) ML1 = 65535;
  if (ML2 > 65535) ML2 = 65535;
  if (MR2 > 65535) MR2 = 65535;
  if (MR1 > 65535) MR1 = 65535;

  if (ML1 < -65535) ML1 = -65535;
  if (ML2 < -65535) ML2 = -65535;
  if (MR2 < -65535) MR2 = -65535;
  if (MR1 < -65535) MR1 = -65535;


  //MR1
  if (MR1 > 0) {
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  } else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, MR1 + 65535);
  }
  //MR2
  if (MR2 > 0) {
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  } else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, MR2 + 65535);
  }

  //ML2
  if (ML2 > 0) {
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  } else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, ML2 + 65535);
  }

  //ML1
  if (ML1 > 0) {
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  } else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, ML1 + 65535);
  }
}
void moveAngle(int angle, int spd) {
  int x = spd * sin(radians(angle));
  int y = spd * cos(radians(angle));
  motor((x + y), (y - x), (-x - y), (x - y));
}
void move_sec(int angle, int sec) {
  for (int i = 0; i < sec; i++) {
    moveAngle(angle, 50000);
    update_all();



  }
}
float convertAngle(float angle) {
  if (angle > 180) return angle - 360;
  if (angle < -180) return angle + 360;
  return angle;
}
void stop() {
  motor(0, 0, 0, 0);
}
void out() {
  float out_angle;
  int out_cnt = 0;
  cheking_out = true;
  if (ldr.front >= OUT_SENS && ldr.right >= OUT_SENS) {
    move_sec(225, 10);
    out_angle = 45;
  } else if (ldr.front >= OUT_SENS && ldr.left >= OUT_SENS) {
    move_sec(135, 10);
    out_angle = -45;
  } else if (ldr.back >= OUT_SENS && ldr.right >= OUT_SENS) {
    move_sec(315, 10);
    out_angle = 135;
  } else if (ldr.back >= OUT_SENS && ldr.left >= OUT_SENS) {
    move_sec(45, 10);
    out_angle = -135;
  } else if (ldr.front >= OUT_SENS) {
    if(shr < shl) move_sec(135, 17);
    else move_sec(225, 17);
    out_angle = 0;
  } else if (ldr.back >= OUT_SENS) {
    if(shr < shl) move_sec(45, 15);
    else move_sec(315, 15);
    out_angle = 180;
  } else if (ldr.right >= OUT_SENS) {
    move_sec(270, 10);
    out_angle = 90;
  } else if (ldr.left >= OUT_SENS) {
    move_sec(90, 10);
    out_angle = -90;
  } else {
    cheking_out = false;
    return;
  }
  while (HS0038.is_ball && abs(out_angle - convertAngle(HS0038.angle)) < 30 && out_cnt < 50) {
    out_cnt++;
    update_all();
    if (move_inside()) stop();
  }
  cheking_out = false;
}
bool move_inside() {
  if (ldr.front >= OUT_SENS && ldr.right >= OUT_SENS) moveAngle(225, 50000);
  else if (ldr.front >= OUT_SENS && ldr.left >= OUT_SENS) moveAngle(135, 50000);
  else if (ldr.back >= OUT_SENS && ldr.right >= OUT_SENS) moveAngle(315, 50000);
  else if (ldr.back >= OUT_SENS && ldr.left >= OUT_SENS) moveAngle(45, 50000);
  else if (ldr.front >= OUT_SENS) moveAngle(180, 50000);
  else if (ldr.back >= OUT_SENS) moveAngle(0, 50000);
  else if (ldr.right >= OUT_SENS) moveAngle(270, 50000);
  else if (ldr.left >= OUT_SENS) moveAngle(90, 50000);
  else return true;
  return false;
}
bool out_detected() {
  if (cheking_out) return false;
  if (ldr.front >= OUT_SENS || ldr.right >= OUT_SENS || ldr.back >= OUT_SENS || ldr.left >= OUT_SENS) return true;
  return false;
}
void update_all() {
  ldr.read();
  if (!out_detected()) {
    HS0038.read();  /////
    angle_robot = gyro.read();
    shl = analogRead(PA0);
    shb = analogRead(PA1);
    shr = analogRead(PA2);
    diffrent = (shl - shr) * 18;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("BA:");
    display.println(HS0038.angle);
    display.print("GY:");
    display.println(angle_robot);  /////
    display.print("SHL:");
    display.println(shl);
    display.print("SHB:");
    display.println(shb);
    display.print("SHR:");
    display.println(shr);
    display.print("DIST:");
    display.println(HS0038.distance);
    display.print("DIF:");
    display.println(diffrent);
    display.print("Role:");
    display.println(digitalRead(PA11) ? "Forward" : "Goaler");
    display.setCursor(90, 0);
    display.print("F:");
    display.print(ldr.front);
    display.setCursor(90, 10);
    display.print("R:");
    display.print(ldr.right);
    display.setCursor(90, 20);
    display.print("B:");
    display.print(ldr.back);
    display.setCursor(90, 30);
    display.print("L:");
    display.print(ldr.left);
    display.display();
  }
  if(digitalRead(PA3)) already_kicked = false;
}
float clamp(float val, float _min, float _max) {
  if (val > _max) return _max;
  if (val < _min) return _min;
  return val;
}
void shoot() {
  if(already_kicked) return;
  for (int i = 0; i < 3; i++) {
    digitalWrite(PC15, 1);
    update_all();
  }
  digitalWrite(PC15, 0);
  already_kicked = true;
}
void forward() {
  out();
  if (!digitalRead(PA3) && HS0038.is_ball && (HS0038.angle < 30 || HS0038.angle > 330)) {
    moveAngle(0, 65000);
    shoot();
  } else if (HS0038.is_ball) {
    float a = HS0038.angle;
    if (a > 180) a -= 360;
    if (a < -180) a += 360;
    float shift = clamp(a * 0.3, -80, 80);
    float forward_v = map(HS0038.distance, 10, 90, 30000, 65000);
    moveAngle(a + shift, forward_v);
  } else {
    if (shb < 900) motor(-30000 + diffrent, -30000 - diffrent, 30000 - diffrent, 30000 + diffrent);
    else if (shb > 1500) motor(30000 + diffrent, 30000 - diffrent, -30000 - diffrent, -30000 + diffrent);
    else motor(diffrent, -diffrent, -diffrent, diffrent);
  }
}
void goalkeeper() {
  int bd = (shb - 1700) * 18;
  float a = HS0038.angle;
  if (a > 180) a -= 360;
  a *= 30000;
  if (a > 30000) a = 30000;
  if (a < -30000) a = -30000;
  if (HS0038.is_ball) {
    motor(bd + a, bd - a, -bd - a, -bd + a);
  } else {
    motor(bd + diffrent, bd - diffrent, -bd - diffrent, -bd + diffrent);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  pinMode(PC15, OUTPUT);


  pinMode(PB9, PWM);
  pinMode(PB8, PWM);
  pinMode(PB7, PWM);
  pinMode(PB6, PWM);



  display.begin(0x2, 0x3c);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(64, 32);
  display.print("arses");
  display.setCursor(64, 42);
  display.print("solgir");
  display.display();
  delay(1000);

  i2c.begin();
}
void loop() {
  update_all();
  if (digitalRead(PA11)) forward();
  else goalkeeper();
}
