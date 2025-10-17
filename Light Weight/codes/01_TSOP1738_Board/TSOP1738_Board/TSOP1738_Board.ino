#include <Wire.h>
#define I2C_ADDRESS 0x09  

uint32_t start_time = 0;
uint8_t i2cBuffer[16];
uint8_t sample_period = 5;

void setup() {
  pinMode(PB1, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PA6, OUTPUT);
  Wire.begin(I2C_ADDRESS);       
  Wire.onRequest(onRequest);
  pinMode(PA4, OUTPUT);

}

void loop() {
  digitalWrite(PA4, 1);
  delay(1);
  for(int i=0; i<8; i++){
    digitalWrite(PB1, (i/4) % 2);
    digitalWrite(PA7, (i/2) % 2);
    digitalWrite(PA6, (i/1) % 2);
    start_time = 0;
    uint8_t pulse = 0;
    // while(digitalRead(PA0) && start_time < 100) start_time++;
    // while(!digitalRead(PA0)) pulse++;
    pulse = pulseIn(PA0, LOW, 5000);
    i2cBuffer[i] = pulse;
    start_time = 0;
    pulse = 0;
    // while(digitalRead(PA1) && start_time < 100) start_time++;
    // while(!digitalRead(PA1)) pulse++;
    pulse = pulseIn(PA1, LOW, 5000);
    i2cBuffer[8+i] = pulse;
  }
  digitalWrite(PA4, 0);
  delay(1);

}

void onRequest() {
  Wire.write(i2cBuffer, 16);
}
