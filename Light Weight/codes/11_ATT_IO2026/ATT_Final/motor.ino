#define MAX_MOTOR 65000

void init_motors() {
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);

  pinMode(PC15, OUTPUT);

  pinMode(PB9, PWM);
  pinMode(PB8, PWM);
  pinMode(PB7, PWM);
  pinMode(PB6, PWM);
}
void motor(int ML1, int ML2, int MR2, int MR1) {
  float correction = clamp(angle_robot + aim, -90, 90);

  ML1 += (correction * 400);
  ML2 += (correction * 400);
  MR2 += (correction * 400);
  MR1 += (correction * 400);
  int maxVal = abs(ML1);

  if (abs(ML2) > maxVal) maxVal = abs(ML2);
  if (abs(MR2) > maxVal) maxVal = abs(MR2);
  if (abs(MR1) > maxVal) maxVal = abs(MR1);
  if (maxVal > MAX_MOTOR) {
    float scale = (float)MAX_MOTOR / maxVal;

    ML1 *= scale;
    ML2 *= scale;
    MR2 *= scale;
    MR1 *= scale;
  }
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
  if (!OLED_EN) sec *= 20;
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
float clamp(float val, float _min, float _max) {
  if (val > _max) return _max;
  if (val < _min) return _min;
  return val;
}
void moveXY(float Vx, float Vy) {
  int Vl1 = Vy + Vx;
  int Vl2 = Vy - Vx;
  int Vr2 = -Vy - Vx;
  int Vr1 = -Vy + Vx;
  motor(Vl1, Vl2, Vr2, Vr1);
}

void comeback() {
  aim = 0;
  moveXY(diffrent, (shb - 1000) * 100);
}