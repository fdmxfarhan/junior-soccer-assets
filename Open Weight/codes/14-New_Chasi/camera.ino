void read_MV() {
  i2c.requestFrom(0x42, 12);
  if (i2c.available() == 12) {
    uint8_t buffer[12];
    for (int i = 0; i < 12; i++) {
  iwdg_feed();  // reload watchdog

      buffer[i] = i2c.read();
    }
    ball_x = buffer[0] | (buffer[1] << 8);
    ball_y = buffer[2] | (buffer[3] << 8);
    yellow_x = buffer[4] | (buffer[5] << 8);
    yellow_y = buffer[6] | (buffer[7] << 8);
    blue_x = buffer[8] | (buffer[9] << 8);
    blue_y = buffer[10] | (buffer[11] << 8);
    ball_angle = degrees(atan2(robot_x - ball_x, robot_y - ball_y));
    yellow_angle = degrees(atan2(robot_x - yellow_x, robot_y - yellow_y));
    blue_angle = degrees(atan2(robot_x - blue_x, robot_y - blue_y));
    ball_distance = sqrt(pow(robot_x - ball_x, 2) + pow(robot_y - ball_y, 2));
    // if (ball_angle < 0) {
    //   ball_angle += 360;
    // }
    if (ball_x == 0 && ball_y == 0) is_ball = false;
    else is_ball = true;
    if (yellow_x == 0 && yellow_y == 0) is_yellow = false;
    else is_yellow = true;
    if (blue_x == 0 && blue_y == 0) is_blue = false;
    else is_blue = true;


    if (Team_Yellow) {
      is_goal = is_yellow;
      goal_x = yellow_x;
      goal_y = yellow_y;
      goal_angle = yellow_angle;
    } else {
      is_goal = is_blue;
      goal_x = blue_x;
      goal_y = blue_y;
      goal_angle = blue_angle;
    }
  }
}
void read_pixy() {
  uint16_t blocks;
  blocks = pixy.getBlocks();
  is_ball_pixy = false;
  is_yellow_pixy = false;
  is_blue_pixy = false;
  if (blocks) {
    for (int j = 0; j < blocks; j++) {
  iwdg_feed();  // reload watchdog

      if (pixy.blocks[j].signature == 1) {
        is_ball_pixy = true;
        ball_x_pixy = pixy.blocks[j].x;
        ball_y_pixy = pixy.blocks[j].y;
        ball_angle_pixy = degrees(atan2(ball_x_pixy - robot_x_pixy, robot_y_pixy - ball_y_pixy));
        ball_distance_pixy = sqrt(pow(robot_x_pixy - ball_x_pixy, 2) + pow(robot_y_pixy - ball_y_pixy, 2));

      } else if (pixy.blocks[j].signature == 2) {
        is_yellow_pixy = true;
        yellow_x_pixy = pixy.blocks[j].x;
        yellow_y_pixy = pixy.blocks[j].y;
        yellow_angle_pixy = degrees(atan2(yellow_x_pixy - robot_x_pixy, robot_y_pixy - yellow_y_pixy));
      } else if (pixy.blocks[j].signature == 3) {
        is_blue_pixy = true;
        blue_x_pixy = pixy.blocks[j].x;
        blue_y_pixy = pixy.blocks[j].y;
        blue_angle_pixy = degrees(atan2(blue_x_pixy - robot_x_pixy, robot_y_pixy - blue_y_pixy));
      }
    }
    if (Team_Yellow) {
      is_goal_pixy = is_yellow_pixy;
      goal_x_pixy = yellow_x_pixy;
      goal_y_pixy = yellow_y_pixy;
      goal_angle_pixy = yellow_angle_pixy;
    } else {
      is_goal_pixy = is_blue_pixy;
      goal_x_pixy = blue_x_pixy;
      goal_y_pixy = blue_y_pixy;
      goal_angle_pixy = blue_angle_pixy;
    }
    if(goal_angle_pixy > 180) goal_angle_pixy -=360;
  }
}