void print_all() {
  if(!OLED_EN) return;
  if (digitalRead(PB4)) {
    mode++;
    mode %= 4;
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
    display.print("Goal: ");
    display.print(goal_angle_pixy);
    display.print("    ");
    display.println(is_goal_pixy);
    display.print("TDAxis: ");
    display.println(robot_angle);
    display.print("goal: ");
    display.println(Team_Yellow ? "Yellow" : "Blue");
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
    display.print("BD: ");
    display.println(ball_distance);
  } else if (mode == 2) {
    display.setCursor(0, 25);
    display.print("SHL: ");
    display.println(shl);
    display.print("SHB: ");
    display.println(shb);
    display.print("SHR: ");
    display.println(shr);
    display.print("cap: ");
    display.println(cap);
  } else if (mode == 3) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Front: ");
    display.println(ldr.front);
    display.print("Rgiht: ");
    display.println(ldr.right);
    display.print("Back: ");
    display.println(ldr.back);
    display.print("Left: ");
    display.println(ldr.left);
    display.print("ID: ");
    display.println(robot_id);
  }
  display.display();
}