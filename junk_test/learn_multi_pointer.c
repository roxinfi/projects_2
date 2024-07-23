#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void calweight(int *box, int number)
{
    int sum = 0;
    for (int i = 0; i < number; i++)
    {
        sum += box[i];
    }
    printf("The sum of the weight is %d\n", sum);
}

void valueassign(int *box, int number)
{

    for (int i = 0; i < number; i++)
    {
        box[i] = (rand() % 100)+40;
    }
}

int main()
{
    srand(time(NULL));
    int number;
    int box[100];

    printf("Enter the number of elements: ");
    scanf("%d", &number);
    valueassign(box, number);
    calweight(box, number);
    for (int i = 0; i < number; i++)
    {
        if (i % 5 == 0)
        {
            printf("\n");
        }
        else
        {
            printf("%3.3d ", box[i]);
        }
    }

}