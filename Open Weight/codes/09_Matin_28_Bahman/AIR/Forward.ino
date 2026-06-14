void forward() {
  out();
  if (BALL_IN_KICKER) {
    goal();
  } else if (is_ball) {
    catch_ball();
  } else {
    moveXY(D*2, (800 - shb) * 200, 0);
  }
}