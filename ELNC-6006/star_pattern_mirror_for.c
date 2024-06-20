#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    int space;
    size = 5;

    for(row=0;row<size;row++)
    {
        for(space = size - row - 1;space>0;space--)
        {
            printf(" ");
        }
        for(collumn=0;collumn<=row;collumn++)
        {
            printf("*");
        }
        printf("\n");
    }
}