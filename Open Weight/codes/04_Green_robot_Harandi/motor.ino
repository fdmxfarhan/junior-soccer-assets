void init_motors(){
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);
  pinMode(PB8, PWM);
  pinMode(PB9, PWM);
  motor(0, 0, 0, 0);
}
void motor(int ML1, int ML2, int MR2, int MR1) {
 if(use_gy){

  ML1 -= robot_angle * 400;
  ML2 -= robot_angle * 400;
  MR2 -= robot_angle * 400;
  MR1 -= robot_angle * 400;
 }

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
  if (ML2 > 0) {
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML2);
  } else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, 65525 + ML2);
  }
  // Motor Left 2
  if (ML1 > 0) {
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML1);
  } else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, 65525 + ML1);
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
  int x = -v * cos(a * M_PI / 180);
  int y = -v * sin(a * M_PI / 180);
  motor((x + y), (x - y), (-x - y), (y - x));
}
void moveXY(float Vx, float Vy, float w) {
  int speed_x = 1000;
  int speed_y = 1000;
  int speed_w = 500;
  int Vl1 = Vx * speed_x + Vy * speed_y - w * speed_w;
  int Vl2 = -Vx * speed_x + Vy * speed_y - w * speed_w;
  int Vr2 = -Vx * speed_x - Vy * speed_y - w * speed_w;
  int Vr1 = Vx * speed_x - Vy * speed_y - w * speed_w;
  motor(Vl1, Vl2, Vr2, Vr1);
}
void stop() {
  motor(0, 0, 0, 0);
}