void shoot_init() {
  pinMode(PA4, INPUT_PULLUP);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
}
void shoot() {
  for (int i = 0; i < 2; i++) {
    update_all();
    move_inside();
    digitalWrite(PC15, 1);
  }
  digitalWrite(PC15, 0);
}