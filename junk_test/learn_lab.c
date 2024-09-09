#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 200

void avgweight(float *avg, float *sumweight, int *numbox)
{
    *avg = *sumweight / *numbox;
}

void weightcal(float *weight, int *numbox, float *sumweight)
{
    float total = 0;
    for(int i = 0; i < *numbox ; i++)
    {
        float length = (rand() % 51) + 25;
        float width = length;
        float volume = length * width * 105; 
        weight[i] = volume * 0.26;
        total = total + weight[i];
    }
    *sumweight = total;
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

int main(void)
{
    srand(time(NULL));
    float weights[SIZE];
    int num_boxes;
    float total_weight = 0;
    float average = 0;
    float *sumweight = &total_weight;
    printf("Please enter the number of boxes whos weight you want to calculate (Between 50 to 200) :");
    scanf("%d",&num_boxes);
    while ((num_boxes<50)||(num_boxes>200))
    {
        printf("Please enter the value of correct range : ");
        scanf("%d", num_boxes);
    }
    weightcal(weights,&num_boxes,sumweight);
    printArray(weights, num_boxes);
    printf("The total weight is: %.2f\n", total_weight);
    avgweight(&average, sumweight, &num_boxes);
    printf("The average weight is %.2f\n",average);
}