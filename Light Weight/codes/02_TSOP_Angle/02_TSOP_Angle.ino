#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
#define SENSOR_COUNT 16

void setup() {
  
  pinMode(PA8, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);

  display.begin(0x3c);
  display.setTextSize(3);
  display.setTextColor(WHITE);
}

void loop() {
  float sumX = 0, sumY = 0;
  for(int i=0; i<16; i++){
    digitalWrite(PA8, (i/1) % 2);
    digitalWrite(PB1, (i/2) % 2);
    digitalWrite(PC14, (i/4) % 2);
    digitalWrite(PC15, (i/8) % 2);
    float angleDeg = i * (360.0 / SENSOR_COUNT);
    float angleRad = angleDeg * (3.1415926 / 180.0);
    int sens = 4095 - analogRead(PA0);
    sumX += sens * cos(angleRad);
    sumY += sens * sin(angleRad);
  }
  float angle = atan2(sumY, sumX) * 180.0 / 3.1415926;
  if (angle < 0) angle += 360.0;

  display.clearDisplay();
  display.drawCircle(64, 32, 20, WHITE);
  display.fillCircle(
        64 + sin(angle * PI / 180) * 25,
        32 - cos(angle * PI / 180) * 25,
        2, WHITE);
  display.display();
}
