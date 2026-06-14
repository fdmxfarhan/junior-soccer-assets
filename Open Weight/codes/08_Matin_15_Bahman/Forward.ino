void forward() {
  out();
  if (BALL_IN_KICKER) {
    goal();
  } else if (is_ball) {
    catch_ball();
  } else {
    moveXY(D, (800 - shb) * 80, 0);
  }
}