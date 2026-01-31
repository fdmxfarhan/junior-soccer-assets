int shoot_cnt = 0;
void shoot_init(int pwm) {
  cap = analogRead(PB0);
  if (robot_id == 1) {
    if (cap < 1400) {
      pwmWrite(PB9, 2000);
    }
    if (cap > 1500) {
      pwmWrite(PB9, 0);
    }
  } else {
    if (cap < 600) {
      pwmWrite(PB9, 2000);
    }
    if (cap > 650) {
      pwmWrite(PB9, 0);
    }
  }
}
void shoot() {
  if (already_shooted) return;
  if (Ball_In_Kicker) {
    digitalWrite(PB15, 1);
    delay(10);
    digitalWrite(PB15, 0);
    already_shooted = true;
  }
  
}