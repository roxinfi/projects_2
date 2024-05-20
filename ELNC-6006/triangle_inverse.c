#include<stdio.h>
int main(void)
{
    int row, column, size;
    printf("enter the size of the triangle: ");
    scanf("%d", &size);

    row = size;
    while (row > 0)
    {
        column = 0;
        while (column < row)
        {
            printf("*");
            column++;
        }
        printf("\n");
        row--;
    }
    
}