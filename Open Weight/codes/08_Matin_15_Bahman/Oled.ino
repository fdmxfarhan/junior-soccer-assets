int page = 0;
void oled_init() {
  if (!OLED_EN) return;
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("");
  display.print(robot_id);
  display.display();
  delay(500);
  display.setTextSize(1);
  display.display();
}

void print_all() {
  if (!OLED_EN) return;
  if (digitalRead(PA12)) {
    page++;
    page %= 3;
    while (digitalRead(PA12)) iwdg_feed();  // reload watchdog
  }
  display.clearDisplay();
  if (page == 0) {
    display.setCursor(0, 20);
    display.print("Xb:");
    display.println(ball_x);
    display.print("Yb: ");
    display.println(ball_y);
    display.print("db: ");
    display.println(ball_distance);
    display.print("id: ");
    display.println(robot_id);
    display.println(Team_Yellow ? "Yellow" : "Blue");
  } else if (page == 1) {
    display.setCursor(0, 20);
    display.print("F:");
    display.println(ldr.front);
    display.print("R:");
    display.println(ldr.right);
    display.print("B:");
    display.println(ldr.back);
    display.print("L:");
    display.println(ldr.left);
  } else if (page == 2) {
    display.setCursor(0, 20);
    display.print("shr:");
    display.println(shr);
    display.print("shb:");
    display.println(shb);
    display.print("shl:");
    display.println(shl);
  }
  display.display();
}