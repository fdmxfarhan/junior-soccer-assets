void sensor() {
  shl = analogRead(PA0);
  shb = analogRead(PA1);
  shr = analogRead(PA2);
  d = (shl - shr) / 4;
  if(BALL_IN_KICKER) BALL_IN_KICKER_Timer++;
  else BALL_IN_KICKER_Timer = 0;
}