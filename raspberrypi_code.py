import RPi.GPIO as GPIO
from picamera import PiCamera
import sys
import numpy as np
import cv2
import time

mtr1 = 20
mtr2 = 21

GPIO.setmode(GPIO.BCM) #broadcom mode pin configuration
GPIO.setup(mtr1, GPIO.OUT)
GPIO.setup(mtr2, GPIO.OUT)

print("Waiting For Sensor To Settle")

def cv_imread(file_path):
    cv_img = cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), -1)
    return cv_img

while True:
    camera=0
    camera = PiCamera()
    camera.start_preview()
    camera.capture('/home/pi/Desktop/image.jpg')
    camera.stop_preview()
    camera.close()

    file_path = '/home/pi/Desktop/image.jpg'
    img = cv_imread(file_path)
    #print(img)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hue = hsv[0][0][0]
    print('the fruit value is',hue)
    time.sleep(0.6)

    if hue>10 and hue<30:
        print('orange  detecting')
        time.sleep(3)
        GPIO.output(mtr1,GPIO.HIGH)
        time.sleep(0.7)
        GPIO.output(mtr1,GPIO.LOW)
        time.sleep(2)
    elif hue>30 and hue<95:
        print('detecting green')
        time.sleep(3)
        GPIO.output(mtr2,GPIO.HIGH)
        time.sleep(0.8)
        GPIO.output(mtr2,GPIO.LOW)
        time.sleep(2)
    else:
        GPIO.output(mtr1,GPIO.LOW)
        GPIO.output(mtr2,GPIO.LOW)
        time.sleep(2)
        print ("not orange")

    print ("no fruit")
print("finish")





