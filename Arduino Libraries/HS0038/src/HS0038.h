#ifndef HS0038_H
#define HS0038_H

#include <Arduino.h>
#include <Wire.h>

class HS0038 {
public:
    HS0038(TwoWire *wire = &Wire, uint8_t address = 0x09);
    void begin(TwoWire *wire = &Wire, uint8_t address = 0x09);
    void read();

    uint8_t data[3];
    bool is_ball = false;
    float angle;
    float distance;
private:
  uint8_t _addr;
  TwoWire *_wire;
  const uint8_t _count = 3;
};

#endif
