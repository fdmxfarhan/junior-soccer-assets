void shoot_init(int pwm) {
  int cap = analogRead(PB0);
  if (robot_id == 1) {
    if (cap < 1399) {
      pwmWrite(PB9, pwm);
    }
    if (cap > 1599) {
      pwmWrite(PB9, 0);
    }
  }else{
    if (cap < 600) {
      pwmWrite(PB9, pwm);
    }
    if (cap > 650) {
      pwmWrite(PB9, 0);
    }
  }
}
void shoot() {
  digitalWrite(PC15, 1);
  delay(25);
  digitalWrite(PC15, 0);
  delay(25);
}