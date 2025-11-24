#include "HS0038.h"
#include <math.h>

HS0038::HS0038(TwoWire *wire, uint8_t address) : _addr(address), _wire(wire) {
}

void HS0038::begin(TwoWire *wire, uint8_t address) {
  _wire = wire;
}

void HS0038::read() {
    uint8_t index = 0;
    _wire->requestFrom(_addr, 3);
    while (_wire->available() && index < 3) {
        data[index] = _wire->read();
        index++;
    }
    if(data[0] == 0xFF && data[1] == 0xFF && data[2] == 0xFF) is_ball = false;
    else {
        is_ball = true;
        angle = ((data[0] << 8) | data[1])/100.0;
        distance = data[2];
    }
    while (_wire->available()) data[index] = _wire->read();
}
