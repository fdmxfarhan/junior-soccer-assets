int out_cnt = 0;
void update_all() {
  robot_angle = gy.read();
  ldr.read();
  print_all();
  read_MV();
  read_pixy();
  sensor();
}
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
  // shoot();
  digitalWrite(PC14, 1);
  if (Ball_in_Kicker_Timer > 30 || ldr.front > 3) {
    shoot();
    Ball_in_Kicker_Timer = 0;
  } else if (is_goal_pixy == true) {
    TDAxis_enable = false;
    motor((goal_x_pixy - robot_x_pixy + goal_y_pixy - robot_y_pixy) * v / 5 + (-goal_angle_pixy * v_gardesh_pixy),
          (-goal_x_pixy + robot_x_pixy + goal_y_pixy - robot_y_pixy) * v / 5 + (-goal_angle_pixy * v_gardesh_pixy),
          (-goal_x_pixy + robot_x_pixy + -goal_y_pixy + robot_y_pixy) * v / 5 + (-goal_angle_pixy * v_gardesh_pixy),
          (goal_x_pixy - robot_x_pixy + -goal_y_pixy + robot_y_pixy) * v / 5 + (-goal_angle_pixy * v_gardesh_pixy));
    if (goal_angle_pixy >= -10 && goal_angle_pixy <= 10) {
      shoot();
    }
  } else if (is_goal == true) {
    TDAxis_enable = false;
    motor((goal_x - robot_x + goal_y - robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (-goal_x + robot_x + goal_y - robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (-goal_x + robot_x + -goal_y + robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (goal_x - robot_x + -goal_y + robot_y) * v / 10 + (-goal_angle * v_gardesh));
    if (goal_angle >= -10 && goal_angle <= 10) {
      shoot();
    }
  } else {
    TDAxis_enable = true;
    // TDAxis_reverse = true;
    // move_angle_speed(180, 100);
    motor(-100 + d * 2, -100 - d * 2, 100 - d * 2, 100 + d * 2);
    // stop();
  }
}
void goal2() {
  // shoot();
  digitalWrite(PC14, 1);
  if (is_goal == true) {
    TDAxis_reverse = false;
    TDAxis_enable = false;
    motor((goal_x - robot_x + goal_y - robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (-goal_x + robot_x + goal_y - robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (-goal_x + robot_x + -goal_y + robot_y) * v / 10 + (-goal_angle * v_gardesh),
          (goal_x - robot_x + -goal_y + robot_y) * v / 10 + (-goal_angle * v_gardesh));
    if (goal_angle >= -10 && goal_angle <= 10) {
      shoot();
    }
  } else {
    TDAxis_enable = true;
    TDAxis_reverse = true;
    // motor(-100 + d * 2, -100 - d * 2, 100 - d * 2, 100 + d * 2);
    motor(100 + d * 2, 100 - d * 2, -100 - d * 2, -100 + d * 2);
  }
}
void No_Rotate_AI() {
  TDAxis_reverse = false;
  if (Ball_In_Kicker) {
    goal2();
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
  TDAxis_reverse = false;
  if (Ball_In_Kicker) {
    goal2();
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
  if (robot_id == 2) {
    if (shb < 400) {
      motor(-200 + d, -200 - d, 200 - d, 200 + d);
    } else if (shb > 600) {
      motor(200 + d, 200 - d, -200 - d, -200 + d);
    } else {
      motor(d, -d, -d, d);
    }
  } else {
    if (shb < 600) {
      motor(-200 + d, -200 - d, 200 - d, 200 + d);
    } else if (shb > 800) {
      motor(200 + d, 200 - d, -200 - d, -200 + d);
    } else {
      motor(d, -d, -d, d);
    }
  }
}
void out(){
  out_cnt = 0;
  if(ldr.front > 3){
    if(Ball_In_Kicker) shoot(); 
    move_sec(180, 10, 300);
    while((is_ball || is_ball_pixy) && out_cnt < 50){
      stop();
      update_all();
      out_cnt++;
    }
  }
  if(ldr.back > 3){
    move_sec(0, 8, 300);
    while((is_ball || is_ball_pixy) && out_cnt < 50){
      stop();
      update_all();
      out_cnt++;
    }
  }
  if(ldr.right > 3){
    move_sec(270, 5, 300);
    while((is_ball || is_ball_pixy) && out_cnt < 50){
      stop();
      update_all();
      out_cnt++;
    }
  }
  if(ldr.left > 3){
    move_sec(90, 5, 300);
    while((is_ball || is_ball_pixy) && out_cnt < 50){
      stop();
      update_all();
      out_cnt++;
    }
  }
}