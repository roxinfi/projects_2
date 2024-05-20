#include<stdio.h>
int main(void)
{
    int row, column, inverse, size;
    size = 5;

    row = size;
    while (row > 0)
    {
        column = 0;
        inverse = size - row;
        while (column < inverse)
        {
            printf(" ");
            column++;
        }
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