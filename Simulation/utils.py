# utils.py
from math import *

def defineVariables(robot):
    robot.robot_x = 0
    robot.robot_y = 0
    robot.robot_angle = 0
    robot.ball_x = 0
    robot.ball_y = 0
    robot.is_ball = False

def updateSensors(robot):
    gps = robot.get_gps_coordinates()
    robot.robot_angle = degrees(robot.get_compass_heading())
    robot.robot_x = gps[0]
    robot.robot_y = gps[1]
    if robot.robot.getName()[0] == 'B':
        robot.robot_x = -robot.robot_x
        robot.robot_y = -robot.robot_y
    if robot.is_new_ball_data():
        robot.is_ball = True
        ball_data = robot.get_new_ball_data()

        ball_angle = degrees(
            atan2(ball_data['direction'][1],
                    ball_data['direction'][0])
        )

        ball_distance = abs(
            0.01666666 /
            (abs(ball_data['direction'][2]) /
                sqrt(1 - ball_data['direction'][2] ** 2))
        )

        robot.ball_x = -sin(radians(ball_angle + robot.robot_angle)) * ball_distance + robot.robot_x
        robot.ball_y =  cos(radians(ball_angle + robot.robot_angle)) * ball_distance + robot.robot_y
    else:
        robot.is_ball = False
    # ارسال اطلاعات به روبات های دیگر
    robot.send_data_to_team({
        "is_ball": robot.is_ball,
        "ball_x": robot.ball_x,
        "ball_y": robot.ball_y,
        "robot_x": robot.robot_x,
        "robot_y": robot.robot_y,
        "id": int(robot.robot.getName()[1])
    })
    # دریافت اطلاعات از ربات های دیگر
    while robot.is_new_team_data(): # تازمانی که دیتای جدیدی از رباتهای دیگر وجود دارد
        team_data = robot.get_new_team_data()['robot_id']
        if not robot.is_ball and team_data['is_ball']:
            robot.ball_x = team_data['ball_x']
            robot.ball_y = team_data['ball_y']
            robot.is_ball = True

def move(robot, tx, ty, max_speed=10):
    target_angle = degrees(atan2(robot.robot_x - tx, ty - robot.robot_y)) - robot.robot_angle
    if target_angle < -180:
        target_angle += 360
    elif target_angle > 180:
        target_angle -= 360

    left_speed = target_angle * 0.6 + max_speed
    right_speed = -target_angle * 0.6 + max_speed

    if left_speed > max_speed:
        left_speed = max_speed
    if left_speed < -max_speed:
        left_speed = -max_speed
    if right_speed > max_speed: 
        right_speed = max_speed
    if right_speed < -max_speed:
        right_speed = -max_speed

    robot.left_motor.setVelocity(left_speed)
    robot.right_motor.setVelocity(right_speed)

def stop(robot):
    robot.left_motor.setVelocity(0)
    robot.right_motor.setVelocity(0)

def getBehindBallPoint(ball_x, ball_y, goal_x, goal_y, offset=0.09):
    dx = goal_x - ball_x
    dy = goal_y - ball_y

    dist = sqrt(dx * dx + dy * dy)
    if dist == 0:
        return ball_x, ball_y

    ux = dx / dist
    uy = dy / dist

    target_x = ball_x - ux * offset
    target_y = ball_y - uy * offset

    return target_x, target_y
def isRobotAheadOfBall(robot, goal_x, goal_y):
    # بردار توپ به دروازه
    v1x = goal_x - robot.ball_x
    v1y = goal_y - robot.ball_y

    # بردار توپ به ربات
    v2x = robot.robot_x - robot.ball_x
    v2y = robot.robot_y - robot.ball_y

    dot = v1x * v2x + v1y * v2y
    return dot > 0
def getSidePoint(ball_x, ball_y, goal_x, goal_y, side=-1, offset=0.2):
    dx = goal_x - ball_x
    dy = goal_y - ball_y

    dist = sqrt(dx * dx + dy * dy)
    if dist == 0:
        return ball_x, ball_y

    # بردار عمود
    px = -dy / dist
    py = dx / dist

    return (
        ball_x + px * offset * side,
        ball_y + py * offset * side
    )
def goAndPushBall(robot, goal_x, goal_y):
    dist_to_ball = sqrt(
        (robot.robot_x - robot.ball_x) ** 2 +
        (robot.robot_y - robot.ball_y) ** 2
    )

    # اگر ربات جلوی توپ است → دور بزن
    if isRobotAheadOfBall(robot, goal_x, goal_y):
        tx, ty = getSidePoint(
            robot.ball_x,
            robot.ball_y,
            goal_x,
            goal_y,
        )
        move(robot, tx, ty)
        return

    # اگر هنوز به توپ نرسیده → برو پشت توپ
    if dist_to_ball > 0.09:
        tx, ty = getBehindBallPoint(
            robot.ball_x,
            robot.ball_y,
            goal_x,
            goal_y
        )
        move(robot, tx, ty)
    else:
        # هل دادن توپ
        move(robot, goal_x, goal_y)
def defensiveFormation(robot):
    robot_id = int(robot.robot.getName()[1])

    OWN_GOAL_X = 0
    OWN_GOAL_Y = -0.7

    if robot_id == 1:
        # جلوی دروازه (Goal cover)
        tx = OWN_GOAL_X
        ty = OWN_GOAL_Y + 0.2

    elif robot_id == 2:
        # دفاع راست
        tx = 0.25
        ty = -0.1

    elif robot_id == 3:
        # دفاع چپ
        tx = -0.25
        ty = -0.1

    else:
        tx = 0
        ty = -0.4

    move(robot, tx, ty)

def goalkeeper(robot):
    GOAL_LINE_Y = -0.55
    MAX_X = 0.3
    MIN_X = -0.3

    # پیش‌فرض: روی خط دروازه
    target_y = GOAL_LINE_Y

    if robot.is_ball:
        target_x = robot.ball_x

        # اگر توپ از خط دروازه‌بان عبور کرد → خروج برای دفع
        if robot.ball_y < GOAL_LINE_Y:
            target_y = robot.ball_y

    else:
        target_x = 0  # وسط دروازه

    # محدود کردن x
    if target_x > MAX_X:
        target_x = MAX_X
    elif target_x < MIN_X:
        target_x = MIN_X

    move(robot, target_x, target_y)
