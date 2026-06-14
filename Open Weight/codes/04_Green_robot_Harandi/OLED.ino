void oled_init(){
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
}

void print_all(){
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("Xb:");
  display.println(ball_x);
  display.print("Yb: ");
  display.println(ball_y);
  display.print("Ab: ");
  display.println(ball_angle);

  display.drawCircle(90, 32, 20, WHITE);
  display.drawLine(90 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 90 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  if(is_ball)  display.fillCircle(90 + 25 * sin(radians(ball_angle)), 32 - 25 * cos(radians(ball_angle)), 2, WHITE);

  display.display();
}