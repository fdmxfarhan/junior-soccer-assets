#define OUT_SENS 3
bool cheking_out = false;

void out() {
  float out_angle;
  int out_cnt = 0;
  cheking_out = true;
  if (ldr.front >= OUT_SENS && ldr.right >= OUT_SENS) {
    move_sec(225, 10);
    out_angle = 45;
  } else if (ldr.front >= OUT_SENS && ldr.left >= OUT_SENS) {
    move_sec(135, 10);
    out_angle = -45;
  } else if (ldr.back >= OUT_SENS && ldr.right >= OUT_SENS) {
    move_sec(315, 10);
    out_angle = 135;
  } else if (ldr.back >= OUT_SENS && ldr.left >= OUT_SENS) {
    move_sec(45, 10);
    out_angle = -135;
  } else if (ldr.front >= OUT_SENS) {
    if (shr < shl) move_sec(135, 17);
    else move_sec(225, 17);
    out_angle = 0;
  } else if (ldr.back >= OUT_SENS) {
    if (shr < shl) move_sec(45, 15);
    else move_sec(315, 15);
    out_angle = 180;
  } else if (ldr.right >= OUT_SENS) {
    move_sec(270, 10);
    out_angle = 90;
  } else if (ldr.left >= OUT_SENS) {
    move_sec(90, 10);
    out_angle = -90;
  } else {
    cheking_out = false;
    return;
  }
  while ( out_cnt < 30 ){ //&& HS0038.is_ball && abs(out_angle - convertAngle(HS0038.angle)) < 30) {
    out_cnt++;
    update_all();
    comeback();
  }
  cheking_out = false;
}

bool move_inside() {
  if (ldr.front >= OUT_SENS && ldr.right >= OUT_SENS) moveAngle(225, 50000);
  else if (ldr.front >= OUT_SENS && ldr.left >= OUT_SENS) moveAngle(135, 50000);
  else if (ldr.back >= OUT_SENS && ldr.right >= OUT_SENS) moveAngle(315, 50000);
  else if (ldr.back >= OUT_SENS && ldr.left >= OUT_SENS) moveAngle(45, 50000);
  else if (ldr.front >= OUT_SENS) moveAngle(180, 50000);
  else if (ldr.back >= OUT_SENS) moveAngle(0, 50000);
  else if (ldr.right >= OUT_SENS) moveAngle(270, 50000);
  else if (ldr.left >= OUT_SENS) moveAngle(90, 50000);
  else return true;
  return false;
}

bool out_detected() {
  if (cheking_out || !is_forward || !HS0038.is_ball) return false;
  if (ldr.front >= OUT_SENS || ldr.right >= OUT_SENS || ldr.back >= OUT_SENS || ldr.left >= OUT_SENS) return true;
  return false;
}