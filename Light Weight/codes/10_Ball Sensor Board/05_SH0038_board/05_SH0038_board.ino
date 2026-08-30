#include <Wire_slave.h>
#define I2C_ADDRESS 0x09  
uint8_t i2cBuffer[3];
uint8_t i2cBufferAll[16];
int sensors[16];
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
  for(int i=0; i<8; i++){
    digitalWrite(PB15, 1);
    //digitalWrite(PC13, 1);
    delay(1);
    digitalWrite(PB14, (i/4)%2);
    digitalWrite(PB13, (i/2)%2);
    digitalWrite(PB12, (i/1)%2);
    delay(1);
    sensors[i+8] = pulseIn(PA1, LOW, 5000);
    delay(1);
    sensors[i] = pulseIn(PA0, LOW, 5000);
    digitalWrite(PB15, 0);
    digitalWrite(PC13, 0);
    delay(4);
  }
  for(int i=0;  i<16; i++){
    i2cBufferAll[i] = sensors[i]/10;
  }
  float sumX = 0, sumY = 0;
  int activeSensorCount = 0;
  int nearestSensor = -1;
  int secondNearestSensor = -1;
  int nearestDistance = 17;
  int secondNearestDistance = 17;
  for(int i=0; i<16; i++){
    if(sensors[i] > 50) {
      activeSensorCount++;

      // Choose active sensors closest to sensor 0 (front) on the circular board.
      int distanceToFront = min(i, 16 - i);
      if(distanceToFront < nearestDistance) {
        secondNearestSensor = nearestSensor;
        secondNearestDistance = nearestDistance;
        nearestSensor = i;
        nearestDistance = distanceToFront;
      } else if(distanceToFront < secondNearestDistance) {
        secondNearestSensor = i;
        secondNearestDistance = distanceToFront;
      }

      float angleDeg = i * 22.5;
      sumX += sensors[i] * cos(radians(angleDeg));
      sumY += sensors[i] * sin(radians(angleDeg));
    }
  }
  if(activeSensorCount == 0){
    i2cBuffer[0] = 0xFF;
    i2cBuffer[1] = 0xFF;
    i2cBuffer[2] = 0xFF;
  }else{
    float angle;
    if(activeSensorCount == 1) {
      angle = nearestSensor * 22.5;
    } else {
      // Average the two selected directions as vectors so sensor 0 and 15
      // correctly average across the 360/0-degree boundary.
      float firstAngle = nearestSensor * 22.5;
      float secondAngle = secondNearestSensor * 22.5;
      float averageX = cos(radians(firstAngle)) + cos(radians(secondAngle));
      float averageY = sin(radians(firstAngle)) + sin(radians(secondAngle));

      // Opposite directions have no unique circular average; use the
      // front-priority sensor as a deterministic fallback.
      if(fabs(averageX) < 0.001 && fabs(averageY) < 0.001) {
        angle = firstAngle;
      } else {
        angle = degrees(atan2(averageY, averageX));
        if(angle < 0) angle += 360.0;
      }
    }

    uint16_t angle_byte = angle;
    i2cBuffer[0] = (angle_byte >> 8) & 0XFF;
    i2cBuffer[1] = angle_byte & 0xFF;
    i2cBuffer[2] = sqrt(pow(sumX, 2) + pow(sumY, 2))/10.0;
  }
}

void onRequest() {
  Wire1.write(i2cBuffer, 3);
  // Wire1.write(i2cBufferAll, 16);
}
