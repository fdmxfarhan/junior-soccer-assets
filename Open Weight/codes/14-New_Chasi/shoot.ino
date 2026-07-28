int shoot_cnt = 0;
void shoot_init(int pwm) {
  cap = analogRead(PB0);
  if (robot_id == 1) {
    if (cap < 1400) {
      pwmWrite(PB9, 1000);
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
  for (int i = 0; i < 5; i++) {
  iwdg_feed();  // reload watchdog

    digitalWrite(PB15, 1);
    update_all();
    // move_angle(0);
  }
  digitalWrite(PB15, 0);
}