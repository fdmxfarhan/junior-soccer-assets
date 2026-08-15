#pragma once
#include <Arduino.h>
#include <math.h>

struct Pose2D {
  float x;
  float y;
  bool valid;
};

// Coordinate system:
//   (0, 0) is the field centre, +x is right, +y is forward.
// robotAngleDeg is 0 while facing forward; positive values turn left.
// Distances and field dimensions must use the same unit (for example, mm).
//
// This expects the right/left sensors to see the two side walls and the back
// sensor to see the rear wall (or front wall when the robot is facing backward).
inline Pose2D localize(float robotAngleDeg,
                       float rightDistance,
                       float leftDistance,
                       float backDistance,
                       float fieldWidth,
                       float fieldHeight) {
  const float angleRad = robotAngleDeg * DEG_TO_RAD;
  const float headingCos = cosf(angleRad);

  // At +/-90 degrees the side sensors are parallel to the side walls, so a
  // reliable x coordinate cannot be calculated using this sensor layout.
  if (fabsf(headingCos) < 0.15f) {
    return {0.0f, 0.0f, false};
  }

  Pose2D pose;

  // Calculate x independently from each side sensor, then average them.
  // The wall seen by the right sensor changes when the robot faces backward.
  const float rightWallX = headingCos > 0.0f
                             ? fieldWidth * 0.5f
                             : -fieldWidth * 0.5f;
  const float leftWallX = -rightWallX;
  const float xFromRight = rightWallX - rightDistance * headingCos;
  const float xFromLeft = leftWallX + leftDistance * headingCos;
  pose.x = (xFromRight + xFromLeft) * 0.5f;

  // With a forward-facing robot the back sensor sees y = -fieldHeight / 2.
  // When the robot faces backward it sees y = +fieldHeight / 2.
  if (headingCos > 0.0f) {
    pose.y = -fieldHeight * 0.5f + backDistance * headingCos;
  } else {
    pose.y = fieldHeight * 0.5f + backDistance * headingCos;
  }

  pose.valid = true;
  return pose;
}
