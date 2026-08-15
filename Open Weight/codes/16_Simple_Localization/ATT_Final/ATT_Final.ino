#include <Wire.h>
#include <TDAxis12.h>
#include <Adafruit_SH1106_STM32.h>
#include <math.h>
#include "localization.h"
#define FIELD_WIDTH 1400
#define FIELD_HEIGHT 1800

// Field drawing area on the 128 x 64 OLED.  Its aspect ratio matches the
// physical field, so the robot's displayed position is not stretched.
const int DISPLAY_WIDTH = 128;
const int DISPLAY_HEIGHT = 64;
const int FIELD_DRAW_HEIGHT = 58;
const int FIELD_DRAW_WIDTH =
  (FIELD_DRAW_HEIGHT * FIELD_WIDTH + FIELD_HEIGHT / 2) / FIELD_HEIGHT;
const int FIELD_DRAW_X = (DISPLAY_WIDTH - FIELD_DRAW_WIDTH) / 2;
const int FIELD_DRAW_Y = (DISPLAY_HEIGHT - FIELD_DRAW_HEIGHT) / 2;
const int ROBOT_RADIUS = 3;
const int ROBOT_EDGE_MARGIN = ROBOT_RADIUS + 1;

Adafruit_SH1106 display(-1);
TwoWire i2c(2, I2C_FAST_MODE);
TDAxis12 gy(&i2c, 0x10);

int speed = 200;
float robot_angle;
int shl, shb, shr;
float robot_x=0, robot_y=0;
bool localization_valid = false;
bool pose_initialized = false;

bool moveToPose(float targetX, float targetY, float targetTheta);

struct TargetPose {
  float x;
  float y;
  float theta;
};

// Edit this list to create a different route. Coordinates are field-centred
// and use mm; theta is degrees (positive values turn left).
const TargetPose poseList[] = {
  {200.0f, 200.0f,   0.0f},
  {200.0f, -200.0f,  0.0f},
  {-200.0f, -200.0f,  0.0f},
  {-200.0f, 200.0f,   0.0f}
};
const uint8_t POSE_COUNT = sizeof(poseList) / sizeof(poseList[0]);
uint8_t activePoseIndex = 0;

float clampFloat(float value, float minimum, float maximum) {
  if (value < minimum) return minimum;
  if (value > maximum) return maximum;
  return value;
}

void drawField() {
  display.drawRect(FIELD_DRAW_X, FIELD_DRAW_Y,
                   FIELD_DRAW_WIDTH, FIELD_DRAW_HEIGHT, WHITE); 
}

void drawRobot(float x, float y, float angleDeg) {
  // Convert the field-centred coordinates to OLED pixels.  OLED y increases
  // downward, so positive field y is drawn upward.
  x = clampFloat(x, -FIELD_WIDTH * 0.5f, FIELD_WIDTH * 0.5f);
  y = clampFloat(y, -FIELD_HEIGHT * 0.5f, FIELD_HEIGHT * 0.5f);

  const float xRatio = (x + FIELD_WIDTH * 0.5f) / FIELD_WIDTH;
  const float yRatio = (FIELD_HEIGHT * 0.5f - y) / FIELD_HEIGHT;
  const int pixelX = FIELD_DRAW_X + ROBOT_EDGE_MARGIN +
    (int)(xRatio * (FIELD_DRAW_WIDTH - 1 - 2 * ROBOT_EDGE_MARGIN) + 0.5f);
  const int pixelY = FIELD_DRAW_Y + ROBOT_EDGE_MARGIN +
    (int)(yRatio * (FIELD_DRAW_HEIGHT - 1 - 2 * ROBOT_EDGE_MARGIN) + 0.5f);

  display.fillCircle(pixelX, pixelY, ROBOT_RADIUS, WHITE);

  // Small heading line: angle 0 points toward the top of the OLED.
  const float angleRad = angleDeg * DEG_TO_RAD;
  int headingX = pixelX + (int)(-sinf(angleRad) * 5.0f);
  int headingY = pixelY + (int)(-cosf(angleRad) * 5.0f);
  headingX = (int)clampFloat(headingX, FIELD_DRAW_X + 1,
                              FIELD_DRAW_X + FIELD_DRAW_WIDTH - 2);
  headingY = (int)clampFloat(headingY, FIELD_DRAW_Y + 1,
                              FIELD_DRAW_Y + FIELD_DRAW_HEIGHT - 2);
  display.drawLine(pixelX, pixelY, headingX, headingY, WHITE);
}

void runPoseLoop() {
  const TargetPose &target = poseList[activePoseIndex];

  // moveToPose() is non-blocking: it is called every loop until the target is
  // reached. Then select the next target, wrapping after the final one.
  if (moveToPose(target.x, target.y, target.theta)) {
    activePoseIndex++;
    if (activePoseIndex >= POSE_COUNT) {
      activePoseIndex = 0;
    }
  }
}

void setup() {
  pinMode(PA8, OUTPUT);
  digitalWrite(PA8, 0);
  Serial1.begin(115200);
  i2c.begin();
  display.begin(0x3c);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  delay(900);
}

void loop() {
  robot_angle = gy.read();
  shl = analogRead(PA0);
  shb = analogRead(PA1);
  shr = analogRead(PA2);

  Pose2D pose = localize(robot_angle, shr, shl, shb,
                         FIELD_WIDTH, FIELD_HEIGHT);
  localization_valid = pose.valid;
  if (pose.valid) {
    robot_x = pose.x;
    robot_y = pose.y;
    pose_initialized = true;
  }

  runPoseLoop();

  display.clearDisplay();
  drawField();
  if (pose.valid) {
    display.setCursor(0, 32);
    display.println(robot_angle);
    display.println(robot_x);
    display.println(robot_y);
    drawRobot(robot_x, robot_y, robot_angle);
  }

  display.display();
}
