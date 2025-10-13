void read_MV()
{
  i2c.requestFrom(0x42, 12); // Request 12 bytes (6 values * 2 bytes each)
  if (i2c.available() == 12)
  {
    uint8_t buffer[12];
    for (int i = 0; i < 12; i++)
    {
      buffer[i] = i2c.read(); // Read each byte into buffer
    }

    // Unpack the data
    ball_x = (int16_t)(buffer[0] | (buffer[1] << 8));
    ball_y = (int16_t)(buffer[2] | (buffer[3] << 8));
    yellow_x = (int16_t)(buffer[4] | (buffer[5] << 8));
    yellow_y = (int16_t)(buffer[6] | (buffer[7] << 8));
    blue_x = (int16_t)(buffer[8] | (buffer[9] << 8));
    blue_y = (int16_t)(buffer[10] | (buffer[11] << 8));

    ball_angle = get_angle(ball_x, ball_y);
    ball_distance = sqrt(pow(ball_x - robot_x, 2) + pow(ball_y - robot_y, 2));

    yellow_angle = get_angle(yellow_x, yellow_y);
    yellow_distance = sqrt(pow(yellow_x - robot_x, 2) + pow(yellow_y - robot_y, 2));

    blue_angle = get_angle(blue_x, blue_y);
    blue_distance = sqrt(pow(blue_x - robot_x, 2) + pow(blue_y - robot_y, 2));

    if (ball_x == 0 && ball_y == 0) is_ball = false;
    else is_ball = true;

    if (yellow_x == 0 && yellow_y == 0) is_yellow = false;
    else is_yellow = true;

    if (blue_x == 0 && blue_y == 0) is_blue = false;
    else is_blue = true;
  }
}
int get_angle(int x, int y)
{
  int angle = atan2(robot_x - x, robot_y - y) * 180 / PI;
  if (angle < 0)
    angle += 360;
  return angle;
}
int singed_angle(int a){
  if(a > 180) return a-360;
  return a;
}