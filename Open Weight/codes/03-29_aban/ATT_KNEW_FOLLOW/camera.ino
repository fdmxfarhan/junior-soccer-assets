void read_MV() {
  i2c.requestFrom(0x42, 12);
  if (i2c.available() == 12) {
    uint8_t buffer[12];
    for (int i = 0; i < 12; i++) {
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
    // if (ball_angle < 0) {
    //   ball_angle += 360;
    // }
    if (ball_x == 0 & ball_y == 0) is_ball = false;
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
  if (blocks) {
    for (int j = 0; j < blocks; j++) {
      is_ball_pixy = true;
      ball_x_pixy = pixy.blocks[j].x;
      ball_y_pixy = pixy.blocks[j].y;
      ball_angle_pixy = degrees(atan2(ball_x_pixy - robot_x_pixy, robot_y_pixy - ball_y_pixy));
    }
  }
}