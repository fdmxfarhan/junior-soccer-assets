#include "TSOP.h"
#include <math.h>

TSOP_Sensor::TSOP_Sensor(TwoWire *wire, uint8_t address) : _addr(address), _wire(wire) {
}

void TSOP_Sensor::begin(TwoWire *wire, uint8_t address) {
  _wire = wire;
}

float TSOP_Sensor::read() {
    const uint8_t READ_LEN = _count;
    uint8_t index = 0;
    float sumX = 0, sumY = 0;
    is_ball = false;
    _wire->requestFrom(_addr, READ_LEN);
    while (_wire->available() && index < READ_LEN) {
        uint8_t data = _wire->read();
        sensors[index] = data;
        if (data > 1) {
            data = 1; // binary presence
            is_ball = true;
        }
        float angleDeg = index * (360.0 / READ_LEN);
        float angleRad = angleDeg * (3.1415926 / 180.0);
        sumX += data * cos(angleRad);
        sumY += data * sin(angleRad);
        index++;
    }
    if (index == 0) return NAN; // no data received
    float a = atan2(sumY, sumX) * 180.0 / 3.1415926;
    if (a < 0) a += 360.0;
    angle = a;
    return angle;
}
