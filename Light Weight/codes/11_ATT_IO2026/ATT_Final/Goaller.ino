bool goalkeeper_come_forward = false;
void goalkeeper() {
  int bd = (shb - 1700) * 25;
  float a = HS0038.angle;
  if (a > 180) a -= 360;
  a *= 3000;
  if (shr > 2000 && a > 0) a = 0;
  if (shl > 2000 && a < 0) a = 0;

  if (!digitalRead(PA3) && HS0038.is_ball && (HS0038.angle < 30 || HS0038.angle > 330)) {
    moveAngle(0, 65000);
    shoot();
    goalkeeper_come_forward = false;
  } else if (ball_stop_cnt > 2500 || goalkeeper_come_forward) {
    forward();
    goalkeeper_come_forward = true;
  } else if (HS0038.is_ball) {
    moveXY(constrain(a, -50000, 50000), bd);
  } else {
    moveXY(diffrent, bd);
    goalkeeper_come_forward = false;
  }
}