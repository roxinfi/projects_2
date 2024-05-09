import RPi.GPIO as GPIO
import time

# Set GPIO pin numbers
MOSFET1_PIN = 17
MOSFET2_PIN = 18
SWITCH_PIN = 27

# Set GPIO mode and pull-up for the switch pin
GPIO.setmode(GPIO.BCM)
GPIO.setup(SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Set MOSFET pins as outputs
GPIO.setup(MOSFET1_PIN, GPIO.OUT)
GPIO.setup(MOSFET2_PIN, GPIO.OUT)

def switch_pressed(channel):
    # Turn MOSFET 1 on, MOSFET 2 off
    GPIO.output(MOSFET1_PIN, GPIO.HIGH)
    GPIO.output(MOSFET2_PIN, GPIO.LOW)

def switch_released(channel):
    # Turn MOSFET 1 off, MOSFET 2 on
    GPIO.output(MOSFET1_PIN, GPIO.LOW)
    GPIO.output(MOSFET2_PIN, GPIO.HIGH)

# Register callbacks for switch events
GPIO.add_event_detect(SWITCH_PIN, GPIO.FALLING, callback=switch_pressed, bouncetime=200)
GPIO.add_event_detect(SWITCH_PIN, GPIO.RISING, callback=switch_released, bouncetime=200)

try:
    while True:
        # Keep the program running
        time.sleep(1)

except KeyboardInterrupt:
    # Clean up GPIO
    GPIO.cleanup()
