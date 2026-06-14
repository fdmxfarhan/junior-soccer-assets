void shoot_init() {
  pinMode(PA3, INPUT_PULLUP);
  pinMode(PC13, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
  pinMode(PA8, PWM);
  pwmWrite(PA8, 0);
}
void shoot(){
  if(already_shooted) return;
  digitalWrite(PC15, 1);
  delay(50);
  digitalWrite(PC15, 0);
  delay(50);
  already_shooted = true;
}