#include<stdio.h>

int main(void)
{

    int TemperatureC, Pressure;;

    Pressure = 1000;
    TemperatureC = 250;

    float TemperatureF;

    TemperatureF = (TemperatureC * 9.0/5.0) + 32;

    printf("The temperature is = %.2f\n",TemperatureF);

}