void shift(int angle) {
  if (angle < 0) angle += 360;
  if (angle <= 30 || angle >= 330) move_angle(angle);
  else if (angle < 180) move_angle(angle + 50);
  else move_angle(angle - 50);
}
void stop() {
  motor(0, 0, 0, 0);
}
void goal() {
  digitalWrite(PC14, 1);
  if (robot_id == 2) {
    TDAxis_enable = true;

    shoot();
  } else if (is_yellow == true) {
    motor((yellow_x - robot_x + yellow_y - robot_y) * v / 4 + (-yellow_angle),
          (-yellow_x + robot_x + yellow_y - robot_y) * v / 4 + (-yellow_angle),
          (-yellow_x + robot_x + -yellow_y + robot_y) * v / 4 + (-yellow_angle),
          (yellow_x - robot_x + -yellow_y + robot_y) * v / 4 + (-yellow_angle));
    if (yellow_angle > -10 && yellow_angle < 10) {
      shoot();
    }
  } else {
    TDAxis_enable = true;
    move_angle(0);
    //motor(-yellow_angle * v_gardesh , -yellow_abgle * v_gardesh , -yellow_angle * v_gardesh , -yellow_angle * v_gardesh)
  }
}
void No_Rotate_AI() {
  float rotate = turn(primery_speed, speed);
  if (digitalRead(PA3) == 1) {
    goal();
  } else if (is_ball_pixy) {
    digitalWrite(PC14, 1);
    shift(ball_angle_pixy);
  } else if (is_ball) {
    digitalWrite(PC14, 1);
    shift(ball_angle);
  } else {
    digitalWrite(PC14, 0);
    TDAxis_enable = true;
    comeBack();
  }
}
void Rotate_Move_AI() {
  if (digitalRead(PA3) == 1) {
    goal();
  } else if (is_ball_pixy) {
    digitalWrite(PC14, 1);
    TDAxis_enable = false;
    int Vx = robot_x_pixy - ball_x_pixy;
    int Vy = ball_y_pixy - robot_y_pixy;
    int w = ball_angle_pixy;
    motor((Vx + Vy) * v_pixy + (-w * v_gardesh_pixy),
          (-Vx + Vy) * v_pixy + (-w * v_gardesh_pixy),
          (-Vx - Vy) * v_pixy + (-w * v_gardesh_pixy),
          (Vx - Vy) * v_pixy + (-w * v_gardesh_pixy));

  } else if (is_ball) {
    digitalWrite(PC14, 1);
    TDAxis_enable = false;
    int Vx = ball_x - robot_x;
    int Vy = ball_y - robot_y;
    int w = ball_angle;
    motor((Vx + Vy) * v + (-w * v_gardesh),
          (-Vx + Vy) * v + (-w * v_gardesh),
          (-Vx + -Vy) * v + (-w * v_gardesh),
          (Vx + -Vy) * v + (-w * v_gardesh));
  } else {
    digitalWrite(PC14, 0);
    comeBack();
    TDAxis_enable = true;
  }
}
void comeBack() {
  // stop();
  if (shb < 400) {
    motor(-200 + d, -200 - d, 200 - d, 200 + d);
  } else if (shb > 600) {
    motor(200 + d, 200 - d, -200 - d, -200 + d);
  } else {
    motor(d, -d, -d, d);
  }
}