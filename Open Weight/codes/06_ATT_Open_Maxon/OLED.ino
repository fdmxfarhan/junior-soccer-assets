void oled_init(){
  robot_id = digitalRead(PA15)+1;
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("");
  display.print(robot_id);
  display.display();
  delay(900);
  display.setTextSize(1);
  display.display();
}

void print_all(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vcap: ");
  display.println(Vcap);
  display.print("Xb:");
  display.println(ball_x);
  display.print("Yb: ");
  display.println(ball_y);

  display.drawCircle(90, 32, 20, WHITE);
  display.drawLine(90 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 90 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  display.display();
}