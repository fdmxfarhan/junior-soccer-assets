#include "LDR.h"
#include <math.h>

LDR::LDR(TwoWire *wire) : _wire(wire) {
}

void LDR::begin(TwoWire *wire) {
  _wire = wire;
}

void LDR::read() {
    uint8_t index = 0;
    _wire->requestFrom(0x20, 5);
    while (_wire->available() && index < 5) {
        data[index] = _wire->read();
        index++;
    }
    left = data[0] + data[1] + data[2] + data[3];
    index = 0;
    _wire->requestFrom(0x21, 5);
    while (_wire->available() && index < 5) {
        data[index] = _wire->read();
        index++;
    }
    right = data[0] + data[1] + data[2] + data[3];
    index = 0;
    _wire->requestFrom(0x22, 5);
    while (_wire->available() && index < 5) {
        data[index] = _wire->read();
        index++;
    }
    front = data[0] + data[1] + data[2] + data[3];
    index = 0;
    _wire->requestFrom(0x23, 5);
    while (_wire->available() && index < 5) {
        data[index] = _wire->read();
        index++;
    }
    back = data[0] + data[1] + data[2] + data[3];
}
