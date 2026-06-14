int goal_time_out = 0;


void update_all() {
  ldr.read();
  if (!out_detected()) {
    robot_angle = gyro.read();
    read_MV();
    print_all();
    shr = analogRead(PA3);
    shb = analogRead(PA2);
    shl = analogRead(PA1);
    D = (shr - shl) * 20;
    if (robot_id == 1) D = (shr - shl) * 20;
    if (robot_id == 2) D = (shr - shl - 250) * 70;
  }
  if ((is_ball && ball_angle < 20 && ball_angle > -20) || BALL_IN_KICKER) SPIN_ON;
  else SPIN_OFF;
  if (!BALL_IN_KICKER) already_shooted = false;
  iwdg_feed();  // reload watchdog
}
void catch_ball() {
  gyro_reverse = false;
  use_gyro = true;
  float shift = clamp(ball_angle * 1.5, -60, 60);
  if(ball_angle > -20 && ball_angle < 20 && ball_distance < 52) moveSec(ball_angle, HIGH_SPEED, 10);
  else move(ball_angle + shift, HIGH_SPEED);
}
void goal() {
  if (already_shooted) {
    if (arrived_to_goal) {
      gyro_reverse = false;
      use_gyro = true;
      goal_detected = true;
      move(0, VERYLOW_SPEED);
      if (robot_angle > -20 && robot_angle < 20 && goal_time_out > 80) already_shooted = false;
      goal_time_out++;
    } else {
      gyro_reverse = true;
      use_gyro = true;
      move(180, LOW_SPEED);
      goal_time_out = 0;
    }
  } else {
    gyro_reverse = false;
    arrived_to_goal = false;
    if (is_goal && SHOOT_TO_GOAL) {  // && goal_detected) {
      use_gyro = false;
      if (goal_angle <= 20 && goal_angle >= -20) {
        for (int i = 0; i < 4; i++) {
          update_all();
          stop();
        }
        shoot();
        already_shooted = true;
      } else moveXY(0, 0, clamp(goal_angle * 3000, -LOW_SPEED, LOW_SPEED));
    } else {
      use_gyro = true;
      already_shooted = true;
      goal_detected = false;
      move(0, HIGH_SPEED);
      shoot();
    }
  }
}
void defend_goal() {
  if (BALL_IN_KICKER) {
    goal();
  } else if (is_ball) {
    if (ball_distance < 70) {
      out();
      catch_ball();
    } else {
      int Vx = (robot_x - ball_x) * 4000;
      int Vy = (800 - shb) * 80;
      if (shl < 400 && Vx < 0) Vx = 0;
      if (shr < 400 && Vx > 0) Vx = 0;
      moveXY(Vx, Vy, 0);
    }
  } else {
    moveXY(D, (800 - shb) * 80, 0);
  }
}
