int catch_ball_cnt = 0;
void forward() {
  if (BALL_IN_KICKER) {
    out();
    goal();
  } else if (is_ball) {
    out();
    // if(catch_ball_cnt < 20) turn_catch();
    // else catch_ball();
    // catch_ball_cnt++;
    catch_ball();
  } else {
    come_back(900);
  }
}
void come_back(int back_distance) {
  use_gyro = true;
  gyro_reverse = false;
  catch_ball_cnt = 0;
  moveXY(D, (back_distance - shb) * 200, 0);
}