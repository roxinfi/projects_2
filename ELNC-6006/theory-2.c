#include<stdio.h>

int main(void)
{
    int temperature;
    int limit;

    printf("Enter the temperature in Celsius: ");
    scanf("%d%i", &temperature, &limit);

    printf("the temperature is %d\n and limit is %i", temperature, limit);
}