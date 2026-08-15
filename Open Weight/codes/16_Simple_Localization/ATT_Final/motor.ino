void motor(int L1, int L2, int R2, int R1) {
  int16_t speeds[4] = { L1, L2, R2, R1 };
  char data[100];
  data[0] = 0xAA;
  data[1] = 0xAA;
  data[2] = 4;  // Num of Motors
  uint16_t cnt = 4 + 3;
  for (uint8_t i = 0; i < 4; i++) {
    data[i + 3] = i;
    data[cnt] = 0 | 0x0F;
    data[cnt + 1] = (uint8_t)((speeds[i] >> 8) & 0xFF);
    data[cnt + 2] = (uint8_t)(speeds[i] & 0xFF);
    data[cnt + 3] = (uint8_t)((4000 >> 8) & 0xFF);
    data[cnt + 4] = (uint8_t)(4000 & 0xFF);
    cnt += 5;
  }
  data[cnt] = Checksum(&data[2], cnt - 2);
  for (int i = 0; i < cnt + 1; i++) {
    Serial1.write(data[i]);
    delayMicroseconds(10);
  }
}
uint8_t Checksum(char *Buf, uint16_t Length) {
  uint8_t sum = 0;
  for (uint16_t i = 0; i < Length; i++) {
    sum ^= *(Buf++);
  }
  return sum;
}
void moveXY(float Vx, float Vy, float w) {
  int Vl1 = Vx - Vy - w;
  int Vl2 = -Vx - Vy - w;
  int Vr2 = -Vx + Vy - w;
  int Vr1 = Vx + Vy - w;
  motor(Vl1, Vl2, Vr2, Vr1);
}

// Position and heading control tuning.  Distances use the same unit as the
// field dimensions (currently mm); motor commands use the moveXY() scale.
const float MOVE_POSITION_TOLERANCE = 20.0f;
const float MOVE_ANGLE_TOLERANCE = 2.0f;
const float MOVE_POSITION_KP = 0.90f;
const float MOVE_ANGLE_KP = 2.0f;

float wrapAngleDeg(float angleDeg) {
  while (angleDeg > 180.0f) angleDeg -= 360.0f;
  while (angleDeg <= -180.0f) angleDeg += 360.0f;
  return angleDeg;
}

void stopMotors() {
  motor(0, 0, 0, 0);
}

// Non-blocking pose controller. Call it repeatedly from loop() until it
// returns true. It translates first, then rotates at the target. Target x/y
// are field-centred; theta is in degrees where 0 is forward and positive
// angles turn left.
bool moveToPose(float targetX, float targetY, float targetTheta) {
  // Do not move until at least one valid position measurement has been made.
  if (!pose_initialized) {
    stopMotors();
    return false;
  }

  const float errorX = robot_x - targetX;
  const float errorY = targetY - robot_y;
  const float distanceError = sqrtf(errorX * errorX + errorY * errorY);
  const float angleError = wrapAngleDeg(robot_angle - targetTheta);

  if (distanceError <= MOVE_POSITION_TOLERANCE && fabsf(angleError) <= MOVE_ANGLE_TOLERANCE) {
    stopMotors();
    return true;
  }

  // Translating requires a current valid position.  Rotating at the target
  // only needs robot_angle, so it can still finish at headings near +/-90
  // degrees where this three-distance localization is intentionally invalid.
  if (distanceError > MOVE_POSITION_TOLERANCE && !localization_valid) {
    stopMotors();
    return false;
  }

  // Desired velocity in field coordinates: +x is right, +y is forward.
  float fieldVx = 0.0f;
  float fieldVy = 0.0f;
  if (distanceError > MOVE_POSITION_TOLERANCE) {
    fieldVx = errorX * MOVE_POSITION_KP;
    fieldVy = errorY * MOVE_POSITION_KP;
  }

  // Convert the field velocity to robot coordinates expected by moveXY().
  // Robot local +x is right and local +y is forward.
  const float headingRad = robot_angle * DEG_TO_RAD;
  const float headingCos = cosf(headingRad);
  const float headingSin = sinf(headingRad);
  float robotVx = fieldVx * headingCos + fieldVy * headingSin;
  float robotVy = -fieldVx * headingSin + fieldVy * headingCos;

  float w = 0.0f;
  if (fabsf(angleError) > MOVE_ANGLE_TOLERANCE) {
    w = angleError * MOVE_ANGLE_KP;
  }

  // A wheel command is a sum of Vx, Vy, and w. Scale them together so no
  // wheel command exceeds the configured maximum command (speed).
  const float commandTotal = fabsf(robotVx) + fabsf(robotVy) + fabsf(w);
  if (commandTotal > speed) {
    const float scale = speed / commandTotal;
    robotVx *= scale;
    robotVy *= scale;
    w *= scale;
  }

  moveXY(robotVx, robotVy, w);
  return false;
}
