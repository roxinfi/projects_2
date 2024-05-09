import time

def delay_5s():
    # Simulate a 5-second delay
    time.sleep(5)

def main():
    while True:
        # Turn on LED1
        print("LED1 ON")
        delay_5s()

        # Turn off LED1 and turn on LED2
        print("LED1 OFF, LED2 ON")
        delay_5s()

        # Turn off LED2 and turn on LED3
        print("LED2 OFF, LED3 ON")
        delay_5s()

        # Turn off LED3
        print("LED3 OFF")

if __name__ == "__main__":
    main()
