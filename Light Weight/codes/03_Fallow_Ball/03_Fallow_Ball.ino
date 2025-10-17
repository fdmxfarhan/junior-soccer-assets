#include <Wire.h>
#include <TSOP.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TSOP_Sensor TSOP(&i2c, 0x09);
TDAxis12 gyro(&i2c, 0x10);

float ball_angle;
float robot_angle;
int speed = 20000;
void motor(int ML1, int ML2, int MR2, int MR1){
  if(ML1 > 65535) ML1 = 65535;
  if(ML1 <-65535) ML1 =-65535;
  if(ML2 > 65535) ML2 = 65535;
  if(ML2 <-65535) ML2 =-65535;
  if(MR2 > 65535) MR2 = 65535;
  if(MR2 <-65535) MR2 =-65535;
  if(MR1 > 65535) MR1 = 65535;
  if(MR1 <-65535) MR1 =-65535;
  // Motor Left 1
  if(ML1 > 0){
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  }
  else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, 65525 + ML1);
  }
  // Motor Left 2
  if(ML2 > 0){
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  }
  else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, 65525 + ML2);
  }
  // Motor Right 2
  if(MR2 > 0){
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  }
  else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, 65525 + MR2);
  }
  // Motor Right 1
  if(MR1 > 0){
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  }
  else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, 65525 + MR1);
  }
}

void setup() {
  // Serial.begin(115200);
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);
  pinMode(PB8, PWM);
  pinMode(PB9, PWM);
  motor(0, 0, 0, 0);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
  i2c.begin();  // Join I2C bus as master
}

void loop() {
  ball_angle = TSOP.read();
  robot_angle = gyro.read();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawCircle(64, 32, 20, WHITE);
  if(TSOP.is_ball) display.fillCircle(65 + 25 * sin(radians(ball_angle)), 32 - 25 * cos(radians(ball_angle)), 2, WHITE);
  display.drawLine(65 + 15 * sin(radians(robot_angle)), 32 - 15 * cos(radians(robot_angle)), 65 - 15 * sin(radians(robot_angle)), 32 + 15 * cos(radians(robot_angle)), WHITE);
  display.display();
  motor(robot_angle*300, robot_angle*300, robot_angle*300, robot_angle*300);

}
