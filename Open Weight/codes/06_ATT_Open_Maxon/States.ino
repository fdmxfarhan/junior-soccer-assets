void update_state() {
  if (out_detected()) {
    state = STATE_OUT;
    return;
  }

  if (BALL_IN_KICKER) {
    state = STATE_GOAL;
    return;
  }

  if (IS_Goaler) {
    if (is_ball) state = STATE_DEFEND;
    else state = STATE_IDLE;
    return;
  }

  if (is_ball) {
    state = STATE_CATCH_BALL;
  } else {
    state = STATE_SEARCH_BALL;
  }
}
