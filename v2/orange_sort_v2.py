import sys
import cv2
import time
import numpy as np
from PIL import Image
import RPi.GPIO as GPIO
from picamera import PiCamera as picam

# Define GPIO to LCD mapping
LCD_RS = 26
LCD_E = 19
LCD_D4 = 13
LCD_D5 = 6
LCD_D6 = 5
LCD_D7 = 11
LED_ON = 12

#Define GPIO to Arduino Input
MTR1_PULSE = 20
MTR2_PULSE = 21
FRUIT_STATE = 16

# Define some device constants
LCD_WIDTH = 16  # Maximum characters per line
LCD_CHR = True
LCD_CMD = False

LCD_LINE_1 = 0x80  # LCD RAM address for the 1st line
LCD_LINE_2 = 0xC0  # LCD RAM address for the 2nd line

# Timing constants
E_PULSE = 0.00005
E_DELAY = 0.00005

# Setup GPIO
GPIO.setmode(GPIO.BCM)  # Use BCM GPIO numbers
GPIO.setup(FRUIT_STATE, GPIO.IN)
GPIO.setup(MTR1_PULSE, GPIO.OUT)
GPIO.setup(MTR2_PULSE, GPIO.OUT)
GPIO.setup(LCD_E, GPIO.OUT)  # E
GPIO.setup(LCD_RS, GPIO.OUT)  # RS
GPIO.setup(LCD_D4, GPIO.OUT)  # DB4
GPIO.setup(LCD_D5, GPIO.OUT)  # DB5
GPIO.setup(LCD_D6, GPIO.OUT)  # DB6
GPIO.setup(LCD_D7, GPIO.OUT)  # DB7
GPIO.setup(LED_ON, GPIO.OUT)  # Backlight enable

# HUE value of fruit
ORNG_HUE1 = 10
ORNG_HUE2 = 30
GRN_HU1 = 30
GRN_HU2 = 95

# Define the image path
FILE_PATH = '/home/pi/Desktop/image.jpg'

# Declare the global variable
ORANGE_COUNT = 0
GREEN_COUNT = 0
ORANGE = 'Orange-'+str(ORANGE_COUNT)
GREEN = 'Green-'+str(GREEN_COUNT)

def lcd_byte(bits, mode):
    # Send byte to data pins
    # bits = data
    # mode = True  for character
    # False for command

    GPIO.output(LCD_RS, mode)  # RS

    # High bits
    GPIO.output(LCD_D4, False)
    GPIO.output(LCD_D5, False)
    GPIO.output(LCD_D6, False)
    GPIO.output(LCD_D7, False)
    if bits & 0x10 == 0x10:
        GPIO.output(LCD_D4, True)
    if bits & 0x20 == 0x20:
        GPIO.output(LCD_D5, True)
    if bits & 0x40 == 0x40:
        GPIO.output(LCD_D6, True)
    if bits & 0x80 == 0x80:
        GPIO.output(LCD_D7, True)

    # Toggle 'Enable' pin
    time.sleep(E_DELAY)
    GPIO.output(LCD_E, True)
    time.sleep(E_PULSE)
    GPIO.output(LCD_E, False)
    time.sleep(E_DELAY)

    # Low bits
    GPIO.output(LCD_D4, False)
    GPIO.output(LCD_D5, False)
    GPIO.output(LCD_D6, False)
    GPIO.output(LCD_D7, False)
    if bits & 0x01 == 0x01:
        GPIO.output(LCD_D4, True)
    if bits & 0x02 == 0x02:
        GPIO.output(LCD_D5, True)
    if bits & 0x04 == 0x04:
        GPIO.output(LCD_D6, True)
    if bits & 0x08 == 0x08:
        GPIO.output(LCD_D7, True)

    # Toggle 'Enable' pin
    time.sleep(E_DELAY)
    GPIO.output(LCD_E, True)
    time.sleep(E_PULSE)
    GPIO.output(LCD_E, False)
    time.sleep(E_DELAY)


def lcd_string(message, style):
    # Send string to display
    # style=1 Left justified
    # style=2 Centred
    # style=3 Right justified

    if style == 1:
        message = message.ljust(LCD_WIDTH, " ")
    elif style == 2:
        message = message.center(LCD_WIDTH, " ")
    elif style == 3:
        message = message.rjust(LCD_WIDTH, " ")

    for i in range(LCD_WIDTH):
        lcd_byte(ord(message[i]), LCD_CHR)


def lcd_init():
    # Initialise display
    lcd_byte(0x33, LCD_CMD)  # Goto 4bit operating Mode
    lcd_byte(0x32, LCD_CMD)  # Goto 4bit operating Mode
    lcd_byte(0x28, LCD_CMD)  # Funtion set: 4-bit, 2 Line, 5X7 Matrix
    lcd_byte(0x0C, LCD_CMD)  # Cursor OFF, display ON, Blink OFF
    lcd_byte(0x06, LCD_CMD)  # Increment cursor (shift cursor to right)
    lcd_byte(0x01, LCD_CMD)  # Clear display screen
    time.sleep(E_DELAY)


def lcd_WELCOME():
    lcd_init()
    lcd_byte(LCD_LINE_1, LCD_CMD)
    lcd_string("_ORANGE SORTING", 1)
    lcd_byte(LCD_LINE_2, LCD_CMD)
    lcd_string("MECHINE", 2)
    time.sleep(2)
    GPIO.output(LED_ON, True)


def lcd_DISPLAY(lcd_line, message, style):
    lcd_byte(lcd_line, LCD_CMD)
    lcd_string(message, style)


def cv_imread(file_path):
    cv_img = cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), -1)
    return cv_img


def capture_image(file_path):
    camera = 0
    camera = PiCamera()
    camera.start_preview()
    camera.capture(FILE_PATH)
    camera.stop_preview()
    camera.close()


def show_image(file_path):
    try:
        Image.open(file_path).show()  # Displaying the image
    except IOError:
        pass


def main():
    capture_image(FILE_PATH)
    show_image(FILE_PATH)
    img = cv_imread(FILE_PATH)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hue = hsv[0][0][0]
    # cv2.imshow('HUE Image', hsv)  # Displaying the image
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    print('the fruit value is', hue)
    time.sleep(0.6)

    if hue > ORNG_HUE1 and hue < ORNG_HUE2:
        print('detected ORANGE colour')
        GPIO.output(MTR1_PULSE, GPIO.HIGH)
        GPIO.output(MTR2_PULSE, GPIO.LOW)
        ORANGE_COUNT += 1
        lcd_DISPLAY(LCD_LINE_1, ORANGE, 1)
        time.sleep(0.3)
        GPIO.output(MTR1_PULSE, GPIO.LOW)
        GPIO.output(MTR2_PULSE, GPIO.LOW)
        time.sleep(.1)
    elif hue > GRN_HU1 and hue < GRN_HU2:
        print('detected GREEN colour')
        GPIO.output(MTR1_PULSE, GPIO.LOW)
        GPIO.output(MTR2_PULSE, GPIO.HIGH)
        GREEN_COUNT += 1
        lcd_DISPLAY(LCD_LINE_2, GREEN, 1)
        time.sleep(0.3)
        GPIO.output(MTR1_PULSE, GPIO.LOW)
        GPIO.output(MTR2_PULSE, GPIO.LOW)
        time.sleep(.1)
    else:
        GPIO.output(MTR1_PULSE, GPIO.LOW)
        GPIO.output(MTR2_PULSE, GPIO.LOW)
        time.sleep(.1)
        print("NOT A ORANGE !!!")

    print("no fruit")

if __name__ == 'main':
    lcd_WELCOME()
    lcd_byte(LCD_LINE_1, LCD_CMD)
    lcd_string(ORANGE, 1)
    lcd_byte(LCD_LINE_2, LCD_CMD)
    lcd_string(GREEN, 1)
    while True:
        if GPIO.input(FRUIT_STATE) == 1:
            main()
    # Clean GPIO and LCD display
    lcd_byte(0x01, LCD_CMD)
    GPIO.output(LED_ON, False)
    GPIO.cleanup()
    print("Program Closed")
