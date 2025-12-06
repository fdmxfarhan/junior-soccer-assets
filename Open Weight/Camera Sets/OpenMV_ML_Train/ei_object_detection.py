# Edge Impulse - OpenMV FOMO Object Detection Example
#
# This work is licensed under the MIT license.
# Copyright (c) 2013-2024 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE

import sensor, image, time, ml, math, uos, gc, pyb
import ustruct

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=100)          # Let the camera adjust.
i2c = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x42)

net = None
labels = None
min_confidence = 0.5

try:
    # load the model, alloc the model file on the heap if we have at least 64K free after loading
    net = ml.Model("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
    raise Exception('Failed to load "trained.tflite", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

try:
    labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
    raise Exception('Failed to load "labels.txt", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

colors = [ # Add more colors if you are detecting more than 7 types of classes at once.
    (255,0,0),
    (0,0,250),
    (250,250,0),
    (255, 255,   0),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

threshold_list = [(math.ceil(min_confidence * 255), 255)]

def fomo_post_process(model, inputs, outputs):
    ob, oh, ow, oc = model.output_shape[0]

    x_scale = inputs[0].roi[2] / ow
    y_scale = inputs[0].roi[3] / oh

    scale = min(x_scale, y_scale)

    x_offset = ((inputs[0].roi[2] - (ow * scale)) / 2) + inputs[0].roi[0]
    y_offset = ((inputs[0].roi[3] - (ow * scale)) / 2) + inputs[0].roi[1]

    l = [[] for i in range(oc)]

    for i in range(oc):
        img = image.Image(outputs[0][0, :, :, i] * 255)
        blobs = img.find_blobs(
            threshold_list, x_stride=1, y_stride=1, area_threshold=1, pixels_threshold=1
        )
        for b in blobs:
            rect = b.rect()
            x, y, w, h = rect
            score = (
                img.get_statistics(thresholds=threshold_list, roi=rect).l_mean() / 255.0
            )
            x = int((x * scale) + x_offset)
            y = int((y * scale) + y_offset)
            w = int(w * scale)
            h = int(h * scale)
            l[i].append((x, y, w, h, score))
    return l
x_ball = 0
y_ball = 0
x_yellow = 0
y_yellow = 0
x_blue = 0
y_blue = 0
clock = time.clock()
while(True):
    clock.tick()

    img = sensor.snapshot()

    for i, detection_list in enumerate(net.predict([img], callback=fomo_post_process)):
        if i == 0: continue  # background class
        if len(detection_list) == 0: continue  # no detections for this class?

        # print("********** %s **********" % labels[i])
        for x, y, w, h, score in detection_list:
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            # print(f"x {center_x}\ty {center_y}\tscore {score}")
            if labels[i] == "Ball":
                img.draw_circle((center_x, center_y, int(w/4)), color=(250,0,0), thickness=3)
                x_ball = x
                y_ball = y
            if labels[i] == "Blue":
                img.draw_rectangle((x, y, w, h), (0,0,250), fill=False, thickness=3)
                x_blue = x
                y_blue = y
            if labels[i] == "Yellow":
                img.draw_rectangle((x, y, w, h), (250,250,0), fill=False, thickness=3)
                x_yellow = x
                y_yellow = y

    try:
        data = ustruct.pack("<hhhhhh", x_ball, y_ball, x_yellow, y_yellow, x_blue, y_blue)
        # print(x_ball, y_ball)
        i2c.send(data)
    except OSError as e:
        print("I2C Error:", e)

    # print(clock.fps(), "fps", end="\n\n")
