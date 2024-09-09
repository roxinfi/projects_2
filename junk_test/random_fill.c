#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE  20

typedef struct
{
    int sensorValue[SIZE];
    int Max;
    int Min;
    float Average;
} sensor_t;

void fillarray(int *arrptr)
{
    for(int i = 0 ;i < SIZE; i++)
    {
        arrptr[i] = (rand() % 26) + 50; 
    }
}

void printArray(int *arrptr)
{
    for (int j = 0; j < SIZE; j++)
    {
        printf("%2d\t",arrptr[j]);
        if( j % 5 == 4)
        {
            printf("\n");
        }
    }
}

float calaverage(int *arrptr)
{
    float sum = 0;
    for (int i = 0; i < SIZE; i++)
    {
        sum += arrptr[i];
    }
    printf("The average value is %.2f\n",(sum/SIZE)); 
}

int max(int *arrptr)
{
    int max = arrptr[0];
    for(int k = 0; k < SIZE; k++)
    {
        if(arrptr[k] > max)
        {
            max = arrptr[k];
        }
    }
    return max;
}

int min(int *arrptr)
{
    int min = arrptr[0];
    for(int l = 0; l < SIZE; l++)
    {
        if(arrptr[l] < min)
        {
            min = arrptr[l];
        }
    }
    return min;
}

int main(void)
{
    srand(time(NULL));
    sensor_t mysensor = {0};
    fillarray(mysensor.sensorValue);
    printArray(mysensor.sensorValue);
    calaverage(mysensor.sensorValue);
    mysensor.Min = min(mysensor.sensorValue);
    mysensor.Max = max(mysensor.sensorValue);
    printf("The Minimum value in the array is %d\n", mysensor.Min);
    printf("The Maximum value in the array is %d\n", mysensor.Max);
}
