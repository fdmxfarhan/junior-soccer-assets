void init_OLED() {
  if (digitalRead(PA12)) OLED_EN = true;
  if (OLED_EN) {
    display.begin(0x2, 0x3c);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(64, 32);
    display.print("iliya");
    display.setCursor(64, 42);
    display.print("solgir");
    display.display();
    delay(1000);
  }
}
void print_all() {
  if (!OLED_EN) return;
  display.clearDisplay();
  display.drawCircle(64, 32, 20, WHITE);
  display.drawLine(
    64 + sin(angle_robot * PI / 180) * 17,
    32 - cos(angle_robot * PI / 180) * 17,
    64 - sin(angle_robot * PI / 180) * 17,
    32 + cos(angle_robot * PI / 180) * 17,
    WHITE);
  if (HS0038.is_ball)
    display.fillCircle(
      64 + sin(HS0038.angle * PI / 180) * 25,
      32 - cos(HS0038.angle * PI / 180) * 25,
      2, WHITE);
  display.setCursor(0, 0);
  // display.print("BA:");
  // display.println(HS0038.angle);
  // display.print("GY:");
  // display.println(angle_robot);  /////
  display.print("SHL:");
  display.println(shl);
  display.print("SHB:");
  display.println(shb);
  display.print("SHR:");
  display.println(shr);
  // display.print("DIST:");
  // display.println(HS0038.distance);
  // display.print("DIF:");
  // display.println(diffrent);
  // display.print("Role:");
  // display.println(digitalRead(PA11) ? "Forward" : "Goaler");
  // display.setCursor(90, 0);
  // display.print("F:");
  // display.print(ldr.front);
  // display.setCursor(90, 10);
  // display.print("R:");
  // display.print(ldr.right);
  // display.setCursor(90, 20);
  // display.print("B:");
  // display.print(ldr.back);
  // display.setCursor(90, 30);
  // display.print("L:");
  // display.print(ldr.left);
  display.display();
}
