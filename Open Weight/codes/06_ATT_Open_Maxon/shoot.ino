void shoot_init() {
  pinMode(PA4, INPUT_PULLUP);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
}

void shoot() {
  if(already_shooted) return;
  digitalWrite(PC15, 1);
  delay(30);
  digitalWrite(PC15, 0);
  delay(30);
  already_shooted = true;
}