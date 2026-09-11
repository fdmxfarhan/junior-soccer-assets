void update_all() {
  ldr.read();
  if (!out_detected()) {
    HS0038.read();  /////
    angle_ball = HS0038.angle;
    angle_robot = gyro.read();
    shl = analogRead(PA0);
    shb = analogRead(PA1);
    shr = analogRead(PA2);
    diffrent = (shl - shr) * 20;
    print_all();
  }
  if (HS0038.is_ball && abs(last_ball_angle - HS0038.angle) < 20) ball_stop_cnt++;
  else ball_stop_cnt = 0;
  aim_to_goal();
}
