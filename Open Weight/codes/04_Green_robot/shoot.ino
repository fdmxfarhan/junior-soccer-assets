void shoot_init(){
  pinMode(PA3, INPUT_PULLUP);
  pinMode(PC13, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
  pinMode(PA8, PWM);
  pwmWrite(PA8, 0);
}
void boost_voltage(){
  Vcap = analogRead(PB0);
  if(Vcap < 700) {
    pwmWrite(PA8, 2000);
    digitalWrite(PC13, 0);
  }
  if(Vcap > 900) {
    pwmWrite(PA8, 0);
    digitalWrite(PC13, 1);
  }
}