void display_init(){
  display.begin(0x3C);
  // display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(2);
}
void print_all(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(robot_angle);
  display.println(ball_x);
  display.println(ball_y);
  display.println(ball_angle);
  display.display();
}