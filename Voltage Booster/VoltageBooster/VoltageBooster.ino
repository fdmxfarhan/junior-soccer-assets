void setup() {
  pinMode(PB1, OUTPUT);
  pinMode(PA6, OUTPUT);
  analogWrite(PA6, 30);
}

void loop() {
  digitalWrite(PB1, 1);
  delay(50);
  digitalWrite(PB1, 0);
  delay(1000);
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
