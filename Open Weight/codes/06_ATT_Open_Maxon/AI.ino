void update_all() {
  robot_angle = gyro.read();
  read_MV();
  print_all();
  if((is_ball && ball_angle < 20 && ball_angle > -20) || BALL_IN_KICKER) SPIN_ON;
  else SPIN_OFF;
  if(!BALL_IN_KICKER) already_shooted = false;
}