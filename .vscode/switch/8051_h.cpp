#include <8051.h>

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_2

void delay_5s() {
    unsigned int i, j;
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 5000; j++) {
            // Adjust these values based on your microcontroller's clock frequency
            // to achieve a 5-second delay
        }
    }
}

void main() {
    // Configure LED pins as outputs
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;

    while(1) {
        // Turn on LED1
        LED1 = 1;
        delay_5s();
        // Turn off LED1 and turn on LED2
        LED1 = 0;
        LED2 = 1;
        delay_5s();
        // Turn off LED2 and turn on LED3
        LED2 = 0;
        LED3 = 1;
        delay_5s();
        // Turn off LED3
        LED3 = 0;
    }
}
