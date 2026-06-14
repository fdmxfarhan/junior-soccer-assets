String cmd = "";
void update_all() {
  digitalWrite(PC15, digitalRead(PA12));
  digitalWrite(PC13, Ball_In_Kicker);
  if(!Ball_In_Kicker) {
    already_shooted = false;
    kick_timer = 0;
    use_gy = false;
  }
  robot_angle = gyro.read();
  shift = constrain(ball_angle * 1.5, -60, 60);
  read_pixy();
  // boost_voltage();
  // read_MV();
  print_all();
}