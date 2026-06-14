int page = 0;
void oled_init(){
  if(!OLED_EN) return;
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
void print_all(){
  if(!OLED_EN) return;
  if(out_detected()) return;
  if(digitalRead(PA12)){
    page++;
    page%=3;
    while(digitalRead(PA12));
  }
  display.clearDisplay();
  if(page == 0){
    display.setCursor(0, 20);
    display.print("Xb:");
    display.println(ball_x);
    display.print("Yb: ");
    display.println(ball_y);
    display.print("db: ");
    display.println(ball_distance);
    display.print("id: ");
    display.println(robot_id);
    display.println(IS_Goaler ? "Goaler" : "Forward");
    display.drawCircle(90, 32, 20, WHITE);
    display.drawLine(90 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 90 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
    display.fillCircle(90 + 24 * sin(radians(ball_angle)), 32 - 24 * cos(radians(ball_angle)), 2, WHITE);
  }
  else if(page == 1){
    display.setCursor(0, 20);
    display.print("F:");
    display.println(ldr.front);
    display.print("R:");
    display.println(ldr.right);
    display.print("B:");
    display.println(ldr.back);
    display.print("L:");
    display.println(ldr.left);
  }
  else if(page == 2){
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