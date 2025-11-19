#include <Wire.h>
// #define I2C_ADDRESS 0x20 // Left
#define I2C_ADDRESS 0x21 // Right
int16_t sensor[4];
uint16_t sensor_offset[4];
uint8_t i2c_buffer[5];
bool lineDetected = false;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(onRequest);
  pinMode(PA4, INPUT);
  set_sensors();
}

void loop() {
  if (digitalRead(PA4)) {
    set_sensors();
    while (digitalRead(PA4))
      ;
  }
  for (int i = 0; i < 4; i++) {
    sensor[i] = readADC(i) - sensor_offset[i];
    if (sensor[i] < 0) sensor[i] = 0;
    if (sensor[i] > 150) {
      lineDetected = true;
    }
  }
  i2c_buffer[0] = sensor[0] / 100;
  i2c_buffer[1] = sensor[1] / 100;
  i2c_buffer[2] = sensor[2] / 100;
  i2c_buffer[3] = sensor[3] / 100;
  i2c_buffer[4] = lineDetected;
}
void set_sensors() {
  sensor_offset[0] = readADC(0);
  sensor_offset[1] = readADC(1);
  sensor_offset[2] = readADC(2);
  sensor_offset[3] = readADC(3);
}
void onRequest() {
  Wire.write(i2c_buffer, 5);
  lineDetected = false;
}

uint16_t readADC(uint8_t channel) {
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
  ADC1->CHSELR = 1 << channel;
  ADC1->CR |= ADC_CR_ADEN;
  while (!(ADC1->ISR & ADC_ISR_ADRDY))
    ;
  ADC1->CR |= ADC_CR_ADSTART;
  while (!(ADC1->ISR & ADC_ISR_EOC))
    ;
  uint16_t value = ADC1->DR;
  return value;
}
