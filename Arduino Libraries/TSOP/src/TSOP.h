#ifndef TSOP_H
#define TSOP_H

#include <Arduino.h>
#include <Wire.h>

class TSOP_Sensor {
public:
    TSOP_Sensor(TwoWire *wire = &Wire, uint8_t address = 0x09);
    void begin(TwoWire *wire = &Wire, uint8_t address = 0x09);
    float read();

    float angle;           // last computed angle
    uint8_t sensors[16];   // raw sensor readings (0-255)
    bool is_ball = false;

private:
  uint8_t _addr;
  TwoWire *_wire;
  const uint8_t _count = 16;
};

#endif
