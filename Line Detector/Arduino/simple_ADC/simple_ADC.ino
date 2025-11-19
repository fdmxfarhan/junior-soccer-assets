int s0, s1, s2, s3;
int s0_set, s1_set, s2_set, s3_Set;
void setup() {
  pinMode(PA7, OUTPUT);
  s0_set = analogRead(PA0);
}



void loop() {
  s0 = analogRead(PA0) - s0_set;
  if(s0 > 1000) digitalWrite(PA7, 1);
  else digitalWrite(PA7, 0);
}
