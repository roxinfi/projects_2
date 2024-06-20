#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    int space;
    size = 5;


    row = 0;
    while(row<size)
    {
        space = size - row;
        collumn = 0;
        while(space<collumn)
        {
            printf(" ");
        }
        printf("*");
        collumn++;
        printf("\n");
        row++;
    }
}