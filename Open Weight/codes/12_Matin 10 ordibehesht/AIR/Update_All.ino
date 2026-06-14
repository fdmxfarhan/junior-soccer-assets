int goal_time_out = 0;


void update_all() {
  ldr.read();
  if (!out_detected()) {
    robot_angle = gyro.read();
    read_MV();
    shr = analogRead(PA3);
    shb = analogRead(PA2);
    shl = analogRead(PA1);
    D = (shr - shl) * 200;
    if (robot_id == 1) D = (shr - shl) * 20;
    if (robot_id == 2) D = (shr - shl - 250) * 70;
  }
  print_all();
  if ((is_ball && ball_angle < 20 && ball_angle > -20) || BALL_IN_KICKER) SPIN_ON;
  else SPIN_OFF;
  if (!BALL_IN_KICKER) already_shooted = false;

  iwdg_feed();  // reload watchdog
}
void catch_ball() {
  gyro_reverse = false;
  use_gyro = true;

  float shift = clamp(ball_angle * 1.6, -65, 65);

  if (ball_distance < 60 && abs(ball_angle) < 20) {
    for (int i = 0; i < 14; i++) {
      if (robot_id == 1) move(ball_angle, HIGH_SPEED);
      if (robot_id == 2) move(ball_angle, HIGH_SPEED);
      update_all();
      out();
    }
  } else {
    move(ball_angle + shift, HIGH_SPEED);
  }
}
void turn_catch() {
  use_gyro = false;
  gyro_reverse = false;
  if (ball_distance < 60 && abs(ball_angle) < 20) {
    for (int i = 0; i < 14; i++) {
      float Vx = (robot_x - ball_x) * 1000;
      float Vy = (robot_y - ball_y) * 1000;
      float w = ball_angle * 400;
      moveXY(Vx, Vy, w);
      update_all();
      out();
    }
  } else {
    float Vx = (robot_x - ball_x) * 1000;
    float Vy = (robot_y - ball_y) * 1000;
    float w = ball_angle * 400;
    moveXY(Vx, Vy, w);
  }
}
void goal() {
  if (already_shooted) {
    if (arrived_to_goal) {
      gyro_reverse = false;
      use_gyro = true;
      goal_detected = true;
      // move(0, VERYLOW_SPEED);
      stop();
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
    if (is_goal && SHOOT_TO_GOAL) {  // && goal_detected){
      use_gyro = false;
      if (goal_angle <= 2 && goal_angle >= -2) {
        for (int i = 0; i < 5; i++) {
          update_all();
          moveXY(0, 0, clamp(goal_angle * 3000, -LOW_SPEED, LOW_SPEED));
          SPIN_OFF;
        }
        stop();
        shoot();
        already_shooted = true;
      } else moveXY(0, 0, clamp(goal_angle * 3000, -LOW_SPEED, LOW_SPEED));
    } else {
      use_gyro = true;
      already_shooted = true;
      goal_detected = false;
      move(0, HIGH_SPEED);
      if(robot_angle > -20 && robot_angle < 20) shoot();
    }
  }
}
int last_ball_x = 185;
float ball_velocity_x = 0;

void defend_goal() {
  use_gyro = true;
  gyro_reverse = false;
  // محاسبه سرعت و پیش‌بینی
  // ball_velocity_x = ball_x - last_ball_x;
  // int predicted_ball_x = ball_x + (ball_velocity_x * 2);
  // last_ball_x = ball_x;

  if (BALL_IN_KICKER) {
    goal();  // یا کدی که برای شوت زدن داری
  } else if (is_ball) {
    if (ball_distance < 65 && ball_angle < 90 && ball_angle > -90) {
      catch_ball();
    } else {
      int Vx = (robot_x - ball_x) * 4500;
      // int target_y = (ball_distance > 150) ? 700 : 820;
      int Vy = (800 - shb) * 200;

      if (shl < 550 && Vx < 0) Vx = 0;
      if (shr < 550 && Vx > 0) Vx = 0;

      moveXY(Vx, Vy, 0);
    }
  } else {
    come_back(800);
  }
}