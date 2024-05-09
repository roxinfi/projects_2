#include<stdio.h>

int main(void)
{
    printf("hello world") ;
    
    int temperatureC;

    temperatureC = 250;;

    float temperatureF;

    temperatureF = (temperatureC * 9/5) + 32;

    printf(" the temperature is = %f",temperatureF);

    return 0;
}