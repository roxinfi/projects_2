#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

typedef struct
{
    char seconds;
    char minutes;
    int hours;
}clock_t;

int main()
{
    clock_t myclock = {};
    while (1)
    {
        Sleep(1000);
        myclock.seconds++;
        if (myclock.seconds == 60)
        {
            myclock.seconds = 0;
            myclock.minutes++;
            if (myclock.minutes == 60)
            {
                myclock.minutes = 0;
                myclock.hours++;
            }
        }
        printf("The current time is %02d:%02d:%02d\033[H",myclock.hours,myclock.minutes,myclock.seconds);
    }
}