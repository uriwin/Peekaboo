import RPi.GPIO as GPIO
import time
import BlynkLib

BLYNK_AUTH = 'fsEd6GNF6MezMY9KkVmsobasAHAM5r-n'

# Initialize Blynk
blynk = BlynkLib.Blynk(BLYNK_AUTH)

# Setup GPIO mode
GPIO.setmode(GPIO.BCM)

# Setup pin numbers
servo_pin = 13
button_pin = 16

# Setup GPIO output and input channels
GPIO.setup(servo_pin, GPIO.OUT)
GPIO.setup(button_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# Setup PWM parameters
pwm = GPIO.PWM(servo_pin, 50)  # 50 Hz frequency
pwm.start(0)

def set_servo_angle(angle):
    duty = angle / 18 + 2
    GPIO.output(servo_pin, True)
    pwm.ChangeDutyCycle(duty)
    time.sleep(1)
    GPIO.output(servo_pin, False)
    pwm.ChangeDutyCycle(0)

def move_up():
    for angle in range(0, 180, 1):
        set_servo_angle(angle)
        time.sleep(0.03)

def move_down():
    for angle in range(180, 0, -1):
        set_servo_angle(angle)
        time.sleep(0.03)

def button_callback(channel):
    if GPIO.input(button_pin):  # button is released
        move_up()
    else:  # button is pressed
        move_down()

# Detect button press
GPIO.add_event_detect(button_pin, GPIO.BOTH, callback=button_callback, bouncetime=300)

# Blynk run
while True:
    blynk.run()