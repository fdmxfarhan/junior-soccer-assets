void print_all() {
  if (digitalRead(PB4)) {
    mode++;
    mode %= 3;
    while (digitalRead(PB4))
      ;
  }
  display.clearDisplay();
  if (mode == 0) {
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Pixy:");

    display.setCursor(0, 25);
    display.setTextSize(1);
    display.print("BXP: ");
    display.println(ball_x_pixy);
    display.print("BYP: ");
    display.println(ball_y_pixy);
    display.print("BAP: ");
    display.println(ball_angle_pixy);
  } else if (mode == 1) {
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("MV:");

    display.setCursor(0, 25);
    display.setTextSize(1);
    display.print("BX: ");
    display.println(ball_x);
    display.print("BY: ");
    display.println(ball_y);
    display.print("BA: ");
    display.println(ball_angle);
  } else if (mode == 2) {
    display.setCursor(0, 25);
    display.print("SHL: ");
    display.println(shl);
    display.print("SHB: ");
    display.println(shb);
    display.print("SHR: ");
    display.println(shr);
    display.print("TDAxis: ");
    display.println(robot_angle);
  }
  display.display();
}