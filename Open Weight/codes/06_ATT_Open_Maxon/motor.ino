#define IN_1 PB15
#define IN_2 PB14
#define IN_3 PB13
#define IN_4 PB12
#define PWM_1 PB9
#define PWM_2 PB8
#define PWM_3 PB7
#define PWM_4 PB6
#define REVERSE_MOTORS true

void init_motors() {
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(PWM_1, PWM);
  pinMode(PWM_2, PWM);
  pinMode(PWM_3, PWM);
  pinMode(PWM_4, PWM);
  motor(0, 0, 0, 0);
}
void motor(int ML1, int ML2, int MR2, int MR1) {
  if (use_gyro) {
    float correction = robot_angle;
    if (gyro_reverse) {
      if (correction >= 0) correction -= 180;
      else correction += 180;
    }
    correction = clamp(correction, -90, 90);
    ML1 += correction * 400;
    ML2 += correction * 400;
    MR2 += correction * 400;
    MR1 += correction * 400;
  }
  if (REVERSE_MOTORS) {
    ML1 = -ML1;
    ML2 = -ML2;
    MR2 = -MR2;
    MR1 = -MR1;
  }
  if(robot_id == 1){
    ML1 *= 0.7;
    ML2 *= 0.7;
    MR1 *= 0.7;
    MR2 *= 0.7;
  }

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
    digitalWrite(IN_1, 0);
    pwmWrite(PWM_1, ML1);
  } else {
    digitalWrite(IN_1, 1);
    pwmWrite(PWM_1, 65525 + ML1);
  }
  // Motor Left 2
  if (ML2 > 0) {
    digitalWrite(IN_2, 0);
    pwmWrite(PWM_2, ML2);
  } else {
    digitalWrite(IN_2, 1);
    pwmWrite(PWM_2, 65525 + ML2);
  }
  // Motor Right 2
  if (MR2 > 0) {
    digitalWrite(IN_3, 0);
    pwmWrite(PWM_3, MR2);
  } else {
    digitalWrite(IN_3, 1);
    pwmWrite(PWM_3, 65525 + MR2);
  }
  // Motor Right 1
  if (MR1 > 0) {
    digitalWrite(IN_4, 0);
    pwmWrite(PWM_4, MR1);
  } else {
    digitalWrite(IN_4, 1);
    pwmWrite(PWM_4, 65525 + MR1);
  }
}
void move(float a, int v) {
  if (a > 360) a -= 360;
  if (a < 0) a += 360;
  int x = v * cos(a * M_PI / 180);
  int y = v * sin(a * M_PI / 180);
  motor((x + y), (x - y), (-x - y), (y - x));
}
void moveXY(float Vx, float Vy, float w) {
  int Vl1 = Vx + Vy + w;
  int Vl2 = -Vx + Vy + w;
  int Vr2 = -Vx - Vy + w;
  int Vr1 = Vx - Vy + w;
  motor(Vl1, Vl2, Vr2, Vr1);
}
void moveSec(float a, int v, float sec) {
  for (int i = 0; i < sec; i++) {
    if(move_inside()) move(a, v);
    update_all();
  }
}
void stop() {
  motor(0, 0, 0, 0);
}
float clamp(float val, float minn, float maxx) {
  if (val > maxx) return maxx;
  if (val < minn) return minn;
  return val;
}