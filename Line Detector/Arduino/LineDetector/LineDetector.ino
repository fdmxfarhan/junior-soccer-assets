#include <Wire.h>
#define I2C_ADDRESS 0x09 
uint8_t i2cBuffer[16];
uint16_t sensor[4];
void setup() {
  // put your setup code here, to run once:
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(onRequest);
  pinMode(PA0, INPUT_ANALOG);
}

void loop() {
  sensor[0] = readADC(0);
  sensor[1] = readADC(1);
  sensor[2] = readADC(2);
  sensor[3] = readADC(3);
}

void onRequest() {
  Wire.write(i2cBuffer, 16);
}

uint16_t readADC(uint8_t channel) {
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
  ADC1->CHSELR = 1 << channel;
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));
  ADC1->CR |= ADC_CR_ADSTART;
  while(!(ADC1->ISR & ADC_ISR_EOC));
  uint16_t value = ADC1->DR;
  return value;
}
