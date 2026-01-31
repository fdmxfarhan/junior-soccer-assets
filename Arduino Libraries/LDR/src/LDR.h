#ifndef LDR_H
#define LDR_H

#include <Arduino.h>
#include <Wire.h>

class LDR {
public:
    LDR(TwoWire *wire = &Wire);
    void begin(TwoWire *wire = &Wire);
    void read();

    uint8_t data[5];
    uint8_t front;
    uint8_t back;
    uint8_t left;
    uint8_t right;
private:
  TwoWire *_wire;
  const uint8_t _count = 3;
};

#endif
