#include<stdio.h>
#include<time.h>
void cal_average(int *strptr, int *avgpty)
{
    int sum = 0;
    for (int j = 0; j < 10; j++)
    {
        sum = sum + strptr[j];
    }
    *avgpty = sum / 10;
}

int main(void)
{
    srand(time(NULL));
    int average = 0;
    int *avgpty = &average;
    int random = (rand() % 51) +25;
    int string[10];
    int *strptr = string;
    for(int i = 0; i < 10; i++)
    {
        string[i] = random;
    }

    cal_average(strptr,avgpty);
    printf("The average is %d", average);
}