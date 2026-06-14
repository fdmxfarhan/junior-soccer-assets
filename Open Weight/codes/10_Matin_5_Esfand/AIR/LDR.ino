#define LDR_SENS 1
#define MOVE_INSIDE_SEC 15
bool checking_out = false;
void out() {
  checking_out = true;
  float out_ball_angle;
  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) {moveSec(225, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 45;}
  else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) {moveSec(135, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 315;}
  else if (ldr.back > LDR_SENS && ldr.right > LDR_SENS) {moveSec(315, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 135;}
  else if (ldr.back > LDR_SENS && ldr.left > LDR_SENS) {moveSec(45, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 225;}
  else if (ldr.front > LDR_SENS && shr < shl) {moveSec(225, HIGH_SPEED, 20); out_ball_angle = 0;}
  else if (ldr.front > LDR_SENS && shr > shl) {moveSec(135, HIGH_SPEED, 20); out_ball_angle = 0;}
  else if (ldr.right > LDR_SENS) {moveSec(270, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 90;}
  else if (ldr.back >= LDR_SENS && shr > shl) {moveSec(45, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 180;}
  else if (ldr.back >= LDR_SENS && shr < shl) {moveSec(315, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 180;}
  else if (ldr.left > LDR_SENS) {moveSec(90, HIGH_SPEED, MOVE_INSIDE_SEC); out_ball_angle = 270;}
  else {
    checking_out = false;
    return;
  }
  while (is_ball && abs(out_ball_angle - ball_angle) < 60) {
     update_all();
    if (move_inside()) stop();
  }
  checking_out = false;
}
bool move_inside() {
  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) move(225, HIGH_SPEED);
  else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) move(135, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS && ldr.right > LDR_SENS) move(315, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS && ldr.left > LDR_SENS) move(45, HIGH_SPEED);
  else if (ldr.front > LDR_SENS) move(180, HIGH_SPEED);
  else if (ldr.right > LDR_SENS) move(270, HIGH_SPEED);
  else if (ldr.back >= LDR_SENS) move(0, HIGH_SPEED);
  else if (ldr.left > LDR_SENS) move(90, HIGH_SPEED);
  else return true;
  return false;
}
bool out_detected() {
  if (ldr.back > LDR_SENS) arrived_to_goal = true;
  if (checking_out || IS_Goaler) return false;
  if (ldr.front > LDR_SENS || ldr.right > LDR_SENS || ldr.back >= LDR_SENS || ldr.left > LDR_SENS) return true;
  return false;
}
