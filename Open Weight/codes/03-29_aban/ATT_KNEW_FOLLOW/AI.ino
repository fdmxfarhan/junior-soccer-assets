void shift(int angle) {
  if (angle < 0) angle += 360;
  if (angle <= 30 || angle >= 330) move_angle(angle);
  else if (angle < 180) move_angle(angle + 50);
  else move_angle(angle - 50);
}

void No_Rotate_AI() {
  float rotate = turn(primery_speed, speed);
  if (digitalRead(PA3) == 1) {
    digitalWrite(PC14, 1);
    if(robot_id == 2){
      TDAxis_enable = true;
      
      shoot();
    }
    else if (is_yellow == true) {
      motor((yellow_x - robot_x + yellow_y - robot_y) * v/4 + (-yellow_angle),
            (-yellow_x + robot_x + yellow_y - robot_y) * v/4 + (-yellow_angle),
            (-yellow_x + robot_x + -yellow_y + robot_y) * v/4 + (-yellow_angle),
            (yellow_x - robot_x + -yellow_y + robot_y) * v/4 + (-yellow_angle));
      if (yellow_angle > -10 && yellow_angle < 10) {
        shoot();
      }
    } else {
      TDAxis_enable = true;
      move_angle(0);
      //motor(-yellow_angle * v_gardesh , -yellow_abgle * v_gardesh , -yellow_angle * v_gardesh , -yellow_angle * v_gardesh)
    }
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
    digitalWrite(PC14, 1);
    if(robot_id == 2){
      TDAxis_enable = true;
      shoot();
    }
    else if (is_yellow == true) {
      motor((yellow_x - robot_x + yellow_y - robot_y) * v/10 + (-yellow_angle),
            (-yellow_x + robot_x + yellow_y - robot_y) * v/10 + (-yellow_angle),
            (-yellow_x + robot_x + -yellow_y + robot_y) * v/10 + (-yellow_angle),
            (yellow_x - robot_x + -yellow_y + robot_y) * v/10 + (-yellow_angle));
      if (yellow_angle > -10 && yellow_angle < 10) {
        shoot();
      }
      
    } else {
      TDAxis_enable = true;
      move_angle(0);
      //motor(-yellow_angle * v_gardesh , -yellow_abgle * v_gardesh , -yellow_angle * v_gardesh , -yellow_angle * v_gardesh)
    }
  } else if (is_ball_pixy) {
    digitalWrite(PC14, 1);
    TDAxis_enable = false;
    // motor(('{+}?}' - ball_x_pixy) * 2, -(robot_x_pixy - ball_x_pixy) * 2, -(robot_x_pixy - ball_x_pixy) * 2, (robot_x_pixy - ball_x_pixy) * 2);
    // motor((ball_y_pixy - robot_y_pixy) * 2, (ball_y_pixy - robot_y_pixy) * 2, -(ball_y_pixy - robot_y_pixy) * 2, -(ball_y_pixy - robot_y_pixy) * 2);
    // motor(-ball_angle_pixy  , -ball_angle_pixy  , -ball_angle_pixy  ,-ball_angle_pixy  );
    motor((robot_x_pixy - ball_x_pixy + ball_y_pixy - robot_y_pixy) * v_pixy + (-ball_angle_pixy * v_gardesh_pixy),
          (ball_x_pixy - robot_x_pixy + ball_y_pixy - robot_y_pixy) * v_pixy + (-ball_angle_pixy * v_gardesh_pixy),
          (ball_x_pixy - robot_x_pixy + -ball_y_pixy + robot_y_pixy) * v_pixy + (-ball_angle_pixy * v_gardesh_pixy),
          (robot_x_pixy - ball_x_pixy + -ball_y_pixy + robot_y_pixy) * v_pixy + (-ball_angle_pixy * v_gardesh_pixy));

  } else if (is_ball) {
    digitalWrite(PC14, 1);
    TDAxis_enable = false;
    //motor((ball_x - robot_x)* 2 ,-(ball_x - robot_x)* 2,-(ball_x - robot_x)*2,( ball_x - robot_x)*2);
    //motor((ball_y - robot_y)*2,(ball_y - robot_y)*2,-(ball_y - robot_y)*2,-(ball_y - robot_y)*2);
    // motor(-ball_angle  , -ball_angle  , -ball_angle  ,-ball_angle  );
    motor((ball_x - robot_x + ball_y - robot_y) * v + (-ball_angle * v_gardesh),
          (-ball_x + robot_x + ball_y - robot_y) * v + (-ball_angle * v_gardesh),
          (-ball_x + robot_x + -ball_y + robot_y) * v + (-ball_angle * v_gardesh),
          (ball_x - robot_x + -ball_y + robot_y) * v + (-ball_angle * v_gardesh));
  } else {
    digitalWrite(PC14, 0);
    comeBack();
    TDAxis_enable = true;
  }
}
void comeBack() {
  if (shb < 400) {
    motor(-200 + d, -200 - d, 200 - d, 200 + d);
  } else if (shb > 600) {
    motor(200 + d, 200 - d, -200 - d, -200 + d);
  } else {
    motor(d, -d, -d, d);
  }
}