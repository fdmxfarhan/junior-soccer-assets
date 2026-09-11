void shoot() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(PC15, 1);
    update_all();
//    out();
  }
  digitalWrite(PC15, 0);
}
