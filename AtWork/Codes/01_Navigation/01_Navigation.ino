#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);

void setup() {
  display.begin(0x2, 0x3C);
  display.setTextSize(2);
  display.setTextColor(WHITE);

  pinMode(PB15, OUTPUT);  //ML1
  pinMode(PB14, OUTPUT);  //ML2
  pinMode(PB13, OUTPUT);  //MR2
  pinMode(PB12, OUTPUT);  //MR1

  pinMode(PB9, PWM);  //ML1
  pinMode(PB8, PWM);  //ML2
  pinMode(PB7, PWM);  //MR2
  pinMode(PB6, PWM);  //MR1
}

void loop() {
  int v = 65000;
  int front_distance = ananlogRead(PA0);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(front_distance);
  display.display();
  if(front_distance > 2000) {
    motor(v, v, v, v);
    delay(3000);
  }
  else motor(v,v,-v,-v);
}

void motor(int ML1, int ML2, int MR2, int MR1) {
  /////////// Motor L1
  if (ML1 > 0) {
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  } else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, 65535 + ML1);
  }
  /////////// Motor L2
  if (ML2 > 0) {
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  } else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, 65535 + ML2);
  }
  /////////// Motor R2
  if (MR2 > 0) {
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  } else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, 65535 + MR2);
  }
  /////////// Motor R1
  if (MR1 > 0) {
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  } else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, 65535 + MR1);
  }
}