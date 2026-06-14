#define LDR_SENS 0
#define MOVE_INSIDE_SEC 6
bool checking_out = false;
void out() {
  checking_out = true;
  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) moveSec(225, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) moveSec(135, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.back > LDR_SENS && ldr.right > LDR_SENS) moveSec(315, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.back > LDR_SENS && ldr.left > LDR_SENS) moveSec(45, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.front > LDR_SENS) moveSec(180, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.right > LDR_SENS) moveSec(270, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.back > LDR_SENS) moveSec(0, HIGH_SPEED, MOVE_INSIDE_SEC);
  else if (ldr.left > LDR_SENS) moveSec(90, HIGH_SPEED, MOVE_INSIDE_SEC);
  else {
    checking_out = false;
    return;
  }
  float out_ball_angle = ball_angle;
  while (is_ball && abs(out_ball_angle - ball_angle) < 30) {
    update_all();
    if (move_inside()) stop();
  }
  checking_out = false;
}
bool move_inside() {
  if (ldr.front > LDR_SENS && ldr.right > LDR_SENS) move(225, HIGH_SPEED);
  else if (ldr.front > LDR_SENS && ldr.left > LDR_SENS) move(135, HIGH_SPEED);
  else if (ldr.back > LDR_SENS && ldr.right > LDR_SENS) move(315, HIGH_SPEED);
  else if (ldr.back > LDR_SENS && ldr.left > LDR_SENS) move(45, HIGH_SPEED);
  else if (ldr.front > LDR_SENS) move(180, HIGH_SPEED);
  else if (ldr.right > LDR_SENS) move(270, HIGH_SPEED);
  else if (ldr.back > LDR_SENS) move(0, HIGH_SPEED);
  else if (ldr.left > LDR_SENS) move(90, HIGH_SPEED);
  else return true;
  return false;
}
bool out_detected() {
  if(ldr.back > LDR_SENS) arrived_to_goal = true;
  if(checking_out || IS_Goaler) return false;
  // ldr.read();
  if (ldr.front > LDR_SENS || ldr.right > LDR_SENS || ldr.back > LDR_SENS || ldr.left > LDR_SENS) return true;
  return false;
}