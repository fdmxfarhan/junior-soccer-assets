void sensor() {
  shl = analogRead(PA0);
  shb = analogRead(PA1);
  shr = analogRead(PA2);
  d = (shl - shr) / 2;
}