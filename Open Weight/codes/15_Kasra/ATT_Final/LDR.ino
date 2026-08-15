#define LDR_SENS 1
#define MOVE_INSIDE_SEC 10
bool checking_out = false;

void out() {
  checking_out = true;
  float out_ball_angle;

  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) {
    moveSec(225, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 45;
  } else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) {
    moveSec(135, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 315;
  } else if (ldr.back > LDR_SENS && ldr.right > LDR_SENS) {
    moveSec(315, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 135;
  } else if (ldr.back > LDR_SENS && ldr.left > LDR_SENS) {
    moveSec(45, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 225;
  } else if (ldr.front > LDR_SENS && shr < shl) {
    moveSec(180 + 20, HIGH_SPEED, 25);
    out_ball_angle = 0;
  } else if (ldr.front > LDR_SENS && shr > shl) {
    moveSec(180 - 20, HIGH_SPEED, 25);
    out_ball_angle = 0;
  } else if (ldr.right > LDR_SENS) {
    moveSec(270, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 90;
  } else if (ldr.back >= LDR_SENS && shr > shl) {
    moveSec(20, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 180;
  } else if (ldr.back >= LDR_SENS && shr < shl) {
    moveSec(-20, HIGH_SPEED, MOVE_INSIDE_SEC);
    out_ball_angle = 180;
  } else if (ldr.left > LDR_SENS) {
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
  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) move_angle_speed(225, HIGH_SPEED);
  else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) move_angle_speed(135, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS && ldr.right > LDR_SENS) move_angle_speed(315, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS && ldr.left > LDR_SENS) move_angle_speed(45, HIGH_SPEED);
  else if (ldr.front > LDR_SENS) move_angle_speed(180, HIGH_SPEED);
  else if (ldr.right > LDR_SENS) move_angle_speed(270, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS) move_angle_speed(0, HIGH_SPEED);
  else if (ldr.left > LDR_SENS) move_angle_speed(90, HIGH_SPEED);
  else return true;
  return false;
}
bool out_detected() {
  if (ldr.back > LDR_SENS) arrived_to_goal = true;
  if (checking_out || IS_Goaller) return false;
  if (ldr.front > LDR_SENS || ldr.right > LDR_SENS || ldr.back >= LDR_SENS || ldr.left > LDR_SENS) return true;
  return false;
}
void moveSec(float a, int v, float sec) {
  // if(!OLED_EN) sec *= 10;
  for (int i = 0; i < sec; i++) {
  iwdg_feed();  // reload watchdog

    // if (ldr.front < LDR_SENS && ldr.back < LDR_SENS && ldr.right < LDR_SENS && ldr.left < LDR_SENS) return;
    move_angle_speed(a, v);
    update_all();
  }
}



// void out() {
//   if (LDR.left > 3) {
//     move_sec(90,5, 250);
//   } else if (LDR.right > 3) {
//     move_sec(270,5, 250);
//   } else if (LDR.front > 3) {
//     move_sec(180,8, 250);
//   } else if (LDR.back > 3) {
//     move_sec(0,8, 250);
//     arrived_to_goal = true;
//   }

// }
