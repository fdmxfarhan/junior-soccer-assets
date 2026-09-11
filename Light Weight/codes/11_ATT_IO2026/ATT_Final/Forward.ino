void forward() {
  if(!digitalRead(PA3) && HS0038.is_ball && (HS0038.angle < 30 || HS0038.angle > 330)){
    moveAngle(0, 50000);
    shoot();
  }
  else if (HS0038.is_ball) {
    out();
    catch_ball();
  } else {
    comeback();
  }
}

void catch_ball() {
  int a = angle_ball;

  // Normalize angle
  if (a < 0) a += 360;
  if (a >= 360) a -= 360;

  // Convert to -180 ... +180
  // + = right
  // - = left
  int error = a;

  if (error > 180) error -= 360;

  if (shb > 1000) {

    moveAngle(a, 48000);
    return;
  }

  // =================================================
  // FRONT
  // =================================================

  if (abs(error) <= 15) {

    moveAngle(0, 48000);
    return;
  }


  // =================================================
  // FRONT-RIGHT
  // =================================================

  if (error > 15 && error <= 90) {

    int move = error + 40;

    if (move >= 360)
      move -= 360;

    moveAngle(move, 30000);
    return;
  }


  // =================================================
  // FRONT-LEFT
  // =================================================

  if (error < -15 && error >= -90) {

    int move = error - 40;

    if (move < 0)
      move += 360;

    moveAngle(move, 30000);
    return;
  }


  // =================================================
  // BACK-RIGHT
  // =================================================

  if (error > 90) {

    // Stronger shift for back ball
    int move = error + 70;

    if (move >= 360)
      move -= 360;

    moveAngle(move, 44000);
    return;
  }


  // =================================================
  // BACK-LEFT
  // =================================================

  if (error < -90) {

    // Stronger shift for back ball
    int move = error - 70;

    if (move < 0)
      move += 360;

    moveAngle(move, 44000);
    return;
  }
}