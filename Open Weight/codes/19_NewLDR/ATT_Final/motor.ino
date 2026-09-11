void motor(int L1, int L2, int R2, int R1) {
  if (TDAxis_enable == true) {
    float correction = robot_angle;
    if (TDAxis_reverse) {
      if (correction <= 0) correction += 180;
      else correction -= 180;
    }
    correction = clamp(correction, -60, 60);
    L1 -= correction * 2;
    L2 -= correction * 2;
    R2 -= correction * 2;
    R1 -= correction * 2;
  }
  int16_t speeds[4] = { L1, L2, R2, R1 };
  char data[100];
  data[0] = 0xAA;
  data[1] = 0xAA;
  data[2] = 4;  // Num of Motors
  uint16_t cnt = 4 + 3;
  for (uint8_t i = 0; i < 4; i++) {
    iwdg_feed();  // reload watchdog

    data[i + 3] = i;
    data[cnt] = 0 | 0x0F;
    data[cnt + 1] = (uint8_t)((speeds[i] >> 8) & 0xFF);
    data[cnt + 2] = (uint8_t)(speeds[i] & 0xFF);
    data[cnt + 3] = (uint8_t)((4000 >> 8) & 0xFF);
    data[cnt + 4] = (uint8_t)(4000 & 0xFF);
    cnt += 5;
  }
  data[cnt] = Checksum(&data[2], cnt - 2);
  for (int i = 0; i < cnt + 1; i++) {
    iwdg_feed();  // reload watchdog

    Serial1.write(data[i]);
    delayMicroseconds(10);
  }
}
uint8_t Checksum(char *Buf, uint16_t Length) {
  uint8_t sum = 0;
  for (uint16_t i = 0; i < Length; i++) {
    iwdg_feed();  // reload watchdog

    sum ^= *(Buf++);
  }
  return sum;
}
void move_angle(int input_angle) {
  int x = -speed * cos(radians(input_angle));  /////////////////// X of angle
  int y = -speed * sin(radians(input_angle));
  motor(x + y, x - y, -x - y, y - x);
}
void move_angle_speed(int input_angle, int sp) {
  int x = -sp * cos(radians(input_angle));  /////////////////// X of angle
  int y = -sp * sin(radians(input_angle));
  motor(x + y, x - y, -x - y, y - x);
}
void move_sec(int a, int sec, int spd) {
  for (int i = 0; i < sec; i++) {
    move_angle_speed(a, spd);
    update_all();
    iwdg_feed();  // reload watchdog
  }
}
void moveXY(float Vx, float Vy, float w) {
  int Vl1 = Vx - Vy - w;
  int Vl2 = -Vx - Vy - w;
  int Vr2 = -Vx + Vy - w;
  int Vr1 = Vx + Vy - w;
  motor(Vl1, Vl2, Vr2, Vr1);
}
float clamp(float val, float minn, float maxx) {
  if (val > maxx) return maxx;
  if (val < minn) return minn;
  return val;
}