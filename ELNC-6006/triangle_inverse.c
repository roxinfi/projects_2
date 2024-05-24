#include<stdio.h>
int main(void)
{
    int row, column, size;
    size = 5;

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