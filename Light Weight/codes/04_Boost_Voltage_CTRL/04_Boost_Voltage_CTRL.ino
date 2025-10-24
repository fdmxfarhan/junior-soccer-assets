
#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
void setup() {
  // put your setup code here, to run once:
  pinMode(PA8, PWM);
  pwmWrite(PA8, 4000);
  pinMode(PC15, OUTPUT);
  display.begin(0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(2);

}

void loop() {
  digitalWrite(PC15, digitalRead(PA12));
  int V_Cap = analogRead(PB0);
  if(V_Cap > 1100) pwmWrite(PA8, 0);
  if(V_Cap < 950) pwmWrite(PA8, 5000);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vcap: ");
  display.println(V_Cap);
  display.display();
}
