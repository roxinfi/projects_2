#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)

#define MAXSIZE 200
#define HEIGHT 10.5

void findmax(float* weights, int number, float *max)
{
    *max = weights[0]; // Set initial value
    for (int i = 1; i < number; i++)
    {
        if (weights[i] > *max)
        {
            *max = weights[i];
        }
    }
}

void findmin(float* weights, int number, float* min)
{
    *min = weights[0]; // Set initial value
    for (int i = 1; i < number; i++)
    {
        if (weights[i] < *min)
        {
            *min = weights[i];
        }
    }
}

float calcweight(float* weights, int number)
{
    float total_weight = 0;
    for (int i = 0; i < number; i++)
    {
        int length_cm = (rand() % 51) + 25;
        int width_cm = length_cm;
        float volume_cm3 = length_cm * width_cm * HEIGHT;
        weights[i] = volume_cm3 * 0.26;
    }

    for (int j = 0; j < number; j++)
    {
        total_weight = total_weight + weights[j];
    }
    return total_weight;
}

void printArray(float* weights, int number)
{
    printf("\n");
    printf("Got it, I will calculate the weight of %d boxes.\n", number);
    printf("...\n");
    printf("Here are the weights of the %d boxes:\n\n", number);
    for (int k = 0; k < number; k++)
    {
        printf("%05.3f\t", weights[k]);
        if (k % 5 == 4)
        {
            printf("\n");
        }
    }
}

float calcAverage(float* weights, int number)
{
    float total_weight = calcweight(weights, number);
    float average = total_weight / number;
    return average;
}

int main()
{
    srand(time(NULL));
    int number;
    float weights[MAXSIZE];
    float maxWeight, minWeight;

    while (1)
    {
        printf("Enter the number of boxes between (50 to 200): ");
        scanf("%d", &number);
        if ((number >= 50) && (number <= 200))
        {
            break;
        }
        else
        {
            printf("Invalid number of boxes. Please enter a number between 50 and 200.\n");
        }
    }

    float total_weight = calcweight(weights, number);
    printArray(weights, number);
    printf("\n");
    printf("\n");
    printf("Total weight of the boxes: %.2f kg\n", total_weight);
    printf("\n");
    printf("Average weight of the boxes: %.2f kg\n", total_weight / number);
    printf("\n");
    
    findmax(weights, number, &maxWeight);
    printf("Maximum weight of the boxes: %.2f kg\n", maxWeight);
    printf("\n");

    findmin(weights, number, &minWeight);
    printf("Minimum weight of the boxes: %.2f kg\n", minWeight);
    printf("\n");

    return 0;
}
