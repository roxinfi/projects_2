#include <stdio.h>
#include <time.h>
#include <conio.h>

void delay(int milliseconds) 
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while((now - then) < pause)
        now = clock();
}

void print_binary(unsigned char num) 
{
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

int main(void) {
    unsigned char counter = 0x09;
    unsigned char led = 0x00;
    unsigned char display = 0x00;

    while (1) 
    {
        led = 0x00;

        while (_kbhit)
        {
            char ch1 = _getch();
            if (ch1 == '1') 
            {
            counter--;
            }
        }

        if (counter == 0x00) 
        {
            counter = 0x09;
        }

        while (_kbhit()) 
        { 
            char ch = _getch(); 
            if (ch == '2') 
            {
                led = 0x00;
                if (led == 0x00) 
                {
                    led = 0x20;
                } else 
                {
                    led = 0x00;
                }
            }
        }

        display = (counter) | (led);
        printf("The counter is: ");
        print_binary(display);
        delay(1000);
    }

    return 0;
}
