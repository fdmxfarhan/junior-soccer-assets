#include <Wire_slave.h>
#define I2C_ADDRESS 0x09  
uint8_t i2cBuffer[3];
uint8_t sensors[16];
uint8_t pulse = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PC13, OUTPUT);
  pinMode(PB12, OUTPUT);
  pinMode(PB13, OUTPUT);
  pinMode(PB14, OUTPUT);
  pinMode(PB15, OUTPUT);
  for(int i=0; i<2; i++){
    digitalWrite(PC13, 1);
    delay(50);
    digitalWrite(PC13, 0);
    delay(100);
  }
  Wire1.begin(I2C_ADDRESS);       
  Wire1.onRequest(onRequest);
}

void loop() {
  digitalWrite(PB15, 1);
  delayMicroseconds(100);
  for(int i=0; i<8; i++){
    digitalWrite(PB14, (i/4)%2);
    digitalWrite(PB13, (i/2)%2);
    digitalWrite(PB12, (i/1)%2);
    sensors[i] = pulseIn(PA0, LOW, 5000);
    sensors[i+8] = pulseIn(PA1, LOW, 5000);
  }
  digitalWrite(PB15, 0);
  delayMicroseconds(100);
  float sumX = 0, sumY = 0;
  for(int i=0; i<16; i++){
    float angleDeg = i * 22.5;
    float angleRad = angleDeg * (3.1415926 / 180.0);
    sumX += sensors[i] * cos(angleRad);
    sumY += sensors[i] * sin(angleRad);
  }
  float angle = degrees(atan2(sumY, sumX));
  if (angle < 0) angle += 360.0;
  uint16_t angle_byte = angle * 100;
  if(sumX == 0 && sumY == 0){
    i2cBuffer[0] = 0xFF;
    i2cBuffer[1] = 0xFF;
    i2cBuffer[2] = 0xFF;
  }else{
    i2cBuffer[0] = angle_byte >> 8;
    i2cBuffer[1] = angle_byte & 0xFF;
    i2cBuffer[2] = sqrt(pow(sumX, 2) + pow(sumY, 2))/10.0;
  }
  
}

void onRequest() {
  Wire1.write(i2cBuffer, 3);
}