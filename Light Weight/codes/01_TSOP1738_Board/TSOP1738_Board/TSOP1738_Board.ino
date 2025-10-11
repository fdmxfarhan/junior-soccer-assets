#include <Wire.h>
#include <cordic.h>
#define I2C_ADDRESS 0x09  

uint16_t angle = 0;
uint16_t start_time = 0;
uint8_t i2cBuffer[2];
uint8_t sample_period = 5;
cordic8 cordic;    //test cordic8, the 8-bit implementation

void setup() {
  pinMode(PB1, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PA6, OUTPUT);
  Wire.begin(I2C_ADDRESS);       
  Wire.onRequest(onRequest);
}

void loop() {
  float sumX = 0, sumY = 0;
  for(int i=0; i<8; i++){
    digitalWrite(PB1, (i/1) % 2);
    digitalWrite(PA7, (i/2) % 2);
    digitalWrite(PA6, (i/4) % 2);
    start_time = millis();
    uint16_t pulse = 0;
    int sensor_angle = 22.5*i;
    while(digitalRead(PA0) && millis() - start_time < sample_period);
    while(!digitalRead(PA0) && millis() - start_time < sample_period) pulse++;
    sumX += cos(radians(sensor_angle)) * pulse;
    sumY += sin(radians(sensor_angle)) * pulse;
  }
  cordic.atan2sqrt(sumX, sumY);
  angle = (float)cordic.angle*360/256;
  // angle = degrees(atan2(sumY, sumX));
  i2cBuffer[0] = (angle >> 8) & 0xFF;
  i2cBuffer[1] = angle & 0xFF;
}

void onRequest() {
  Wire.write(i2cBuffer, 2);
}
