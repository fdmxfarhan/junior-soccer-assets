#define goal_keeper_back_distance 500
int goal_time_out = 0;
int comeBack_cnt = 0;

void update_all() {
  // ldr.read();
  robot_angle = gy.read();
  read_MV();
  sensor();
  // if (!out_detected()) {
  //   read_pixy();
  // }
  print_all();
  if (!BALL_IN_KICKER) already_shooted = false;
  if ((is_ball && abs(ball_angle) < 40) || (is_ball_pixy && abs(ball_angle_pixy) < 40) || BALL_IN_KICKER) SPIN_ON;
  else SPIN_OFF;
  if (is_ball || is_ball_pixy || BALL_IN_KICKER) comeBack_cnt = 0;
  iwdg_feed();  // reload watchdog
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
  if (already_shooted) {
    //////////////////////////////////////////// Backward Move
    if (arrived_to_goal) {
      TDAxis_reverse = false;
      TDAxis_enable = true;
      goal_detected = true;
      //////////////////////////////////////////// Wait to Charge
      // stop();
      move_angle_speed(180, VERYLOW_SPEED);
      if (robot_angle > -20 && robot_angle < 20 && goal_time_out > 80) already_shooted = false;
      goal_time_out++;
    } else {
      TDAxis_reverse = true;
      TDAxis_enable = true;
      move_angle_speed(180, LOW_SPEED);
      goal_time_out = 0;
    }
  } else {
    TDAxis_reverse = false;
    arrived_to_goal = false;
    //////////////////////////////////////////// Pixy Aim
    // if (is_goal_pixy && SHOOT_TO_GOAL) {
    //   TDAxis_enable = false;
    //   if (goal_angle_pixy <= 20 && goal_angle_pixy >= -20) {
    //     stop();
    //     shoot();
    //     already_shooted = true;
    //   } else moveXY(0, 0, clamp(goal_angle_pixy * 2, -LOW_SPEED, LOW_SPEED));
    // }
    // //////////////////////////////////////////// OpenMV Aim
    // else
    if (is_goal && SHOOT_TO_GOAL) {
      TDAxis_enable = false;
      if (goal_angle <= 4 && goal_angle >= -4) {
        for (int i = 0; i < 5; i++) {
          update_all();
          moveXY(0, 0, clamp(goal_angle, -LOW_SPEED, LOW_SPEED));
          SPIN_OFF;
          iwdg_feed();  // reload watchdog
        }
        stop();
        shoot();
        already_shooted = true;
      } else moveXY(0, 0, clamp(goal_angle, -LOW_SPEED, LOW_SPEED));
    }
    //////////////////////////////////////////// No Goal
    else {
      TDAxis_enable = true;
      goal_detected = false;
      // update_all();
      if (abs(robot_angle) < 5 && abs(d) < 100) {
        shoot();
        // already_shooted = true;
      } 
      else if (abs(robot_angle) > 50) move_angle_speed(robot_angle, LOW_SPEED);
      else moveXY(d*2, LOW_SPEED, robot_angle);
    }
  }
}
void goal_old() {
  digitalWrite(PC14, 1);
  if (BALL_IN_KICKER_Timer > 30) {
    shoot();
    BALL_IN_KICKER_Timer = 0;
  } else if (is_goal_pixy) {
    TDAxis_enable = false;
    motor((goal_x_pixy - robot_x + goal_y_pixy - robot_y) * v / 10 + (-goal_angle_pixy * v_gardesh),
          (-goal_x_pixy + robot_x + goal_y_pixy - robot_y) * v / 10 + (-goal_angle_pixy * v_gardesh),
          (-goal_x_pixy + robot_x + -goal_y_pixy + robot_y) * v / 10 + (-goal_angle_pixy * v_gardesh),
          (goal_x_pixy - robot_x + -goal_y_pixy + robot_y) * v / 10 + (-goal_angle_pixy * v_gardesh));
    if (goal_angle_pixy > -10 && goal_angle_pixy < 10) {
      digitalWrite(PC14, 0);
      shoot();
    }
  } else if (is_goal) {
    TDAxis_enable = false;
    motor((goal_x - robot_x + goal_y - robot_y) * 0.3 + (-goal_angle * 1),
          (-goal_x + robot_x + goal_y - robot_y) * 0.3 + (-goal_angle * 1),
          (-goal_x + robot_x + -goal_y + robot_y) * 0.3 + (-goal_angle * 1),
          (goal_x - robot_x + -goal_y + robot_y) * 0.3 + (-goal_angle * 1));
    if (goal_angle > -10 && goal_angle < 10) {
      digitalWrite(PC14, 0);
      shoot();
    }
  } else {
    TDAxis_enable = true;
    TDAxis_reverse = true;
    move_angle_speed(180, 100);
    // motor(-200 + d, -200 - d, 200 - d, 200 + d);
    // stop();
  }
}
void No_Rotate_AI() {

  TDAxis_reverse = false;
  if (BALL_IN_KICKER) {
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
    comeBack(600);
  }
}
void Rotate_Move_AI() {
  // out();
  TDAxis_reverse = false;
  if (BALL_IN_KICKER) {
    goal();
  } else if (is_ball) {
    if (ball_distance < OUT_OF_SIGHT && abs(ball_angle) <= 20) {
      for (int i = 0; i < 10; i++) {
        iwdg_feed();  // reload watchdog

        TDAxis_enable = false;
        int Vx = 0;  ////ball_x - robot_x;
        int Vy = ball_y - robot_y;
        int w = ball_angle;
        motor((Vx + Vy) * 2.0 + (-w * 1.5),
              (-Vx + Vy) * 2.0 + (-w * 1.5),
              (-Vx - Vy) * 2.0 + (-w * 1.5),
              (Vx - Vy) * 2.0 + (-w * 1.5));
        update_all();
        out();
      }
    } else {
      TDAxis_enable = false;
      int Vx = ball_x - robot_x;
      int Vy = ball_y - robot_y;
      int w = ball_angle;
      motor((Vx + Vy) * v + (-w * v_gardesh),
            (-Vx + Vy) * v + (-w * v_gardesh),
            (-Vx - Vy) * v + (-w * v_gardesh),
            (Vx - Vy) * v + (-w * v_gardesh));
    }
  } else if (is_ball_pixy) {
    TDAxis_enable = false;
    int Vx = robot_x_pixy - ball_x_pixy;
    int Vy = ball_y_pixy - robot_y_pixy;
    int w = ball_angle_pixy;
    motor((Vx + Vy) * v_pixy + (-w * v_gardesh_pixy),
          (-Vx + Vy) * v_pixy + (-w * v_gardesh_pixy),
          (-Vx - Vy) * v_pixy + (-w * v_gardesh_pixy),
          (Vx - Vy) * v_pixy + (-w * v_gardesh_pixy));

  } else {
    if (robot_id == 2) comeBack(900);
    else comeBack(600);
  }
}
void comeBack(int back_distance) {
  if (comeBack_cnt < 30) stop();
  else {
    TDAxis_enable = true;
    TDAxis_reverse = false;
    if (abs(robot_angle) > 70) stop();
    else moveXY(d, (back_distance - shb), 0);
  }
  comeBack_cnt++;
}
void comeBack_old() {
  TDAxis_enable = true;
  if (shb < 400) {
    motor(-200 + d, -200 - d, 200 - d, 200 + d);
  } else if (shb > 600) {
    motor(200 + d, 200 - d, -200 - d, -200 + d);
  } else {
    motor(d, -d, -d, d);
  }
}
void GoalKeeper() {
  TDAxis_enable = true;
  TDAxis_reverse = false;

  if (BALL_IN_KICKER) {
    goal();  // یا کدی که برای شوت زدن داری
  } else if (is_ball) {
    if (ball_distance < 65 && ball_angle < 90 && ball_angle > -90) {
      out();
      Rotate_Move_AI();
    } else {
      int Vx = -(robot_x - ball_x) * 5;
      // int target_y = (ball_distance > 150) ? 700 : 820;
      int Vy = (goal_keeper_back_distance - shb) / 3;

      if (shl < 550 && Vx < 0) Vx = 0;
      if (shr < 550 && Vx > 0) Vx = 0;

      moveXY(Vx, Vy, 0);
    }
  } else if (is_ball_pixy) {
    if (ball_distance_pixy < 65 && ball_angle < 90 && ball_angle > -90) {
      Rotate_Move_AI();
    } else {
      int Vx = (robot_x_pixy - ball_x_pixy) * 5;
      // int target_y = (ball_distance > 150) ? 700 : 820;
      int Vy = (goal_keeper_back_distance - shb);

      if (shl < 550 && Vx < 0) Vx = 0;
      if (shr < 550 && Vx > 0) Vx = 0;

      moveXY(Vx, Vy, 0);
    }
  } else {
    comeBack(goal_keeper_back_distance);
  }
}
