void oled_init(){
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
}

void print_all(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vcap: ");
  display.println(Vcap);
  display.print("speed: ");
  display.println(speed);
  display.println(vl1);
  display.println(vl2);
  display.println(vr2);
  display.println(vr1);

  display.drawCircle(90, 32, 20, WHITE);
  display.drawLine(90 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 90 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  display.display();
}