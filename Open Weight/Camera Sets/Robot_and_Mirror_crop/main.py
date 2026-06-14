import sensor
import time
import pyb
import ustruct

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=100)
# sensor.set_auto_gain(True)  # must be turned off for color tracking
sensor.set_auto_whitebal(True)  # must be turned off for color tracking
sensor.set_auto_exposure(False, exposure_us=5000)  # Adjust exposure time in microseconds
sensor.set_auto_gain(False, gain_db=10)
# Initialize I2C
i2c = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x42)

# Values to send
x_ball = 0
y_ball = 0
x_yellow = 0
y_yellow = 0
x_blue = 0
y_blue = 0

robot_x = 160
robot_y = 111
robot_radius = 38
mirror_radius = 150

clock = time.clock()

ball_color = [62, 38, 61]
yellow_color = [80, -18, 74]
blue_color = [41, -3, -31]

ball_sens = 20
yellow_sens = 18
blue_sens = 8

threshold_Ball = [
    ball_color[0] - ball_sens, ball_color[0] + ball_sens,
    ball_color[1] - ball_sens, ball_color[1] + ball_sens,
    ball_color[2] - ball_sens, ball_color[2] + ball_sens,
]
threshold_Yellow = [
    yellow_color[0] - yellow_sens, yellow_color[0] + yellow_sens,
    yellow_color[1] - yellow_sens, yellow_color[1] + yellow_sens,
    yellow_color[2] - yellow_sens, yellow_color[2] + yellow_sens,
]
threshold_Blue = [
    blue_color[0] - blue_sens, blue_color[0] + blue_sens,
    blue_color[1] - blue_sens, blue_color[1] + blue_sens,
    blue_color[2] - blue_sens, blue_color[2] + blue_sens,
]
tr = [
    threshold_Ball,
    threshold_Yellow,
    threshold_Blue
]

def bigest(blobs):
    m = blobs[0].area()
    b = blobs[0]
    for blob in blobs:
        if blob.area() > m:
            m = blob.area()
            b = blob
    return b

while True:
    clock.tick()
    img = sensor.snapshot()
    img.draw_circle(robot_x, robot_y, robot_radius, (0,100,0), fill=True)
    img.draw_circle(robot_x, robot_y, mirror_radius, (0,100,0), fill=False, thickness = 100)
    yellow_blobs = img.find_blobs([threshold_Yellow], pixels_threshold=10, area_threshold=10, merge=True, margin=10)
    blue_blobs = img.find_blobs([threshold_Blue], pixels_threshold=10, area_threshold=10, merge=True, margin=10)
    orenge_blobs = img.find_blobs([threshold_Ball], pixels_threshold=1, area_threshold=1, merge=True, margin=10)

    # if img.():
    #     print('clicked')
    if len(yellow_blobs) > 0:
        goaly = bigest(yellow_blobs)
        img.draw_rectangle(goaly.rect(), (250,250,0), fill=True)
        x_yellow = int(goaly.x() + goaly.w()/2)
        y_yellow = int(goaly.y() + goaly.h()/2)
    else:
        x_yellow = 0
        y_yellow = 0
    if len(blue_blobs) > 0:
        goalb = bigest(blue_blobs)
        img.draw_rectangle(goalb.rect(), (0,0,250), fill=True)
        x_blue = int(goalb.x() + goalb.w()/2)
        y_blue = int(goalb.y() + goalb.h()/2)
    else:
        x_blue = 0
        y_blue = 0
    if len(orenge_blobs) > 0:
        ball = bigest(orenge_blobs)
        r = int(ball.w())
        x_ball = int(ball.x() + ball.w()/2)
        y_ball = int(ball.y() + ball.h()/2)
        img.draw_circle(x_ball, y_ball, r, (0,255,0), fill=False)
    else:
        x_ball = 0
        y_ball = 0

    try:
        data = ustruct.pack("<hhhhhh", x_ball, y_ball, x_yellow, y_yellow, x_blue, y_blue)
        # print(xb, yb)
        i2c.send(data)
    except OSError as e:
        print("I2C Error:", e)
