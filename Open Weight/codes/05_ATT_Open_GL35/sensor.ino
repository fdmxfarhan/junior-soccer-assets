void sensor() {
  shl = analogRead(PA0);
  shb = analogRead(PA1);
  shr = analogRead(PA2);
  d = (shl - shr) / 2;
  if(Ball_In_Kicker) Ball_in_Kicker_Timer++;
  else Ball_in_Kicker_Timer = 0;
}