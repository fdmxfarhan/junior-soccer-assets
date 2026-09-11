
#define MOVE_INSIDE_SEC 10
bool checking_out = false;

void out() {
  checking_out = true;
  float out_ball_angle;

  if (ldr_front && ldr_right) {
    moveSec(225, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 45;
  } else if (ldr_front && ldr_left) {
    moveSec(135, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 315;
  } else if (ldr_back && ldr_right) {
    moveSec(315, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 135;
  } else if (ldr_back && ldr_left) {
    moveSec(45, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 225;
  } else if (ldr_front && shr < shl) {
    moveSec(180 + 20, HIGH_SPEED, 25);
    out_ball_angle = 0;
  } else if (ldr_front && shr > shl) {
    moveSec(180 - 20, HIGH_SPEED, 25);
    out_ball_angle = 0;
  } else if (ldr_right) {
    moveSec(270, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 90;
  } else if (ldr_back && shr > shl) {
    moveSec(20, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 180;
  } else if (ldr_back && shr < shl) {
    moveSec(-20, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 180;
  } else if (ldr_left) {
    moveSec(90, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 270;
  } else {
    checking_out = false;
    return;
  }
  int cnt = 0;
  while (is_ball && abs(out_ball_angle - ball_angle) < 60 && cnt < 50) {
    update_all();
    if (move_inside()) stop();
    cnt++;
  }
  checking_out = false;
}
bool move_inside() {
  if (ldr_front && ldr_right) move_angle_speed(225, HIGH_SPEED);
  else if (ldr_front && ldr_left) move_angle_speed(135, HIGH_SPEED);
  else if (ldr_back && ldr_right) move_angle_speed(315, HIGH_SPEED);
  else if (ldr_back && ldr_left) move_angle_speed(45, HIGH_SPEED);
  else if (ldr_front) move_angle_speed(180, HIGH_SPEED);
  else if (ldr_right) move_angle_speed(270, HIGH_SPEED);
  else if (ldr_back) move_angle_speed(0, HIGH_SPEED);
  else if (ldr_left) move_angle_speed(90, HIGH_SPEED);
  else return true;
  return false;
}
bool out_detected() {
  if (ldr_back) arrived_to_goal = true;
  if (checking_out || IS_Goaller) return false;
  if (ldr_front || ldr_right || ldr_back || ldr_left) return true;
  return false;
}
void moveSec(float a, int v, float sec) {
  // if(!OLED_EN) sec *= 10;
  for (int i = 0; i < sec; i++) {
  iwdg_feed();  // reload watchdog

    // if (ldr_front && ldr_back && ldr_right && ldr_left) return;
    move_angle_speed(a, v);
    update_all();
  }
}

