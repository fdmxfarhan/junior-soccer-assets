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
    bool front;
    bool back;
    bool left;
    bool right;
private:
  TwoWire *_wire;
  const uint8_t _count = 3;
};

#endif
