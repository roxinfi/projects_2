#include<stdio.h>

int main(void)
{

    float TemperatureC;

    TemperatureC = 250;

    float TemperatureF, Pressure;

    TemperatureF = (TemperatureC * 9.0/5.0) + 32;
    Pressure = (TemperatureC - TemperatureF) * 0.95;

    printf("\n\n\nThe temperature is = %3.2f\n\n",TemperatureF);
    printf("the temperature is = %3.2f\n\n",TemperatureC);
    printf("the relation with pressure is = %3.2f\n\n",Pressure);

}