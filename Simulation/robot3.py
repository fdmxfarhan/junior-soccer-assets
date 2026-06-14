from rcj_soccer_robot import RCJSoccerRobot, TIME_STEP
from utils import *

GOAL_X = 0
GOAL_Y = 0.7

class MyRobot3(RCJSoccerRobot):
    def run(self):
        defineVariables(self)
        while self.robot.step(TIME_STEP) != -1:
            updateSensors(self)

            if self.is_ball:
                goAndPushBall(self, GOAL_X, GOAL_Y)
            else:
                defensiveFormation(self)