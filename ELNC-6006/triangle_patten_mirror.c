#include<stdio.h>
int main(void)
{
    int size, row, colum, space;
    size = 5;

    row = 0;
    while (row < size)
    {
        colum = 0;
        space = size - row;
        while (space > 0)
        {
            printf(" ");
            space--;
        }
        while (colum <= row)
        {
            printf("*");
            colum++;
        }
        printf("\n");
        row++;

    }
}