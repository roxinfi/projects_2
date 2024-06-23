#include<stdio.h>
int main(void)
{
    int xaxis;
    int yaxis;
    int row;
    int column;
    int size;

    printf("Enter the size of the target from 1 to 9: ");
    scanf("%d", &size);
    printf("\n");

    while (size < 1 || size > 9)
    {
        printf("Invalid size. Please enter a size from 1 to 9: ");
        scanf("%d", &size);
        printf("\n");
    }


    printf("Enter the x-axis of the target: ");
    scanf("%d", &xaxis);
    printf("\n");

    while (xaxis < 1 || xaxis > 9)
    {
        printf("Invalid x-axis. Please enter a x-axis from 0 to 9: ");
        scanf("%d", &xaxis);
        printf("\n");
    }

    printf("Enter the y-axis of the target: ");
    scanf("%d", &yaxis);
    printf("\n");

    while (yaxis < 1 || yaxis > 9)
    {
        printf("Invalid y-axis. Please enter a y-axis from 0 to 9: ");
        scanf("%d", &yaxis);
        printf("\n");
    }

    row = 1;
    while(row <= size)
    {
        printf("row%2d  ",row);
        column = 1;
        while(column <= size)
        {
            if(row == xaxis && column == yaxis)
            {
                printf("X");
            }
            else
            {
                printf("O");
            }
            column++;
        }
        printf("\n");
        row++;
    }
    
}