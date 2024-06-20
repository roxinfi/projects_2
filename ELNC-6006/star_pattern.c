#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    int space;
    size = 5;


    row = size;
    while(row>0)
    {
        space = size - row;
        while(space>0)
        {
            printf(" ");
            space--;
        }
        collumn = 0;
        while(collumn<=row-1)
        {
            printf("*");
            collumn++;
        }
        printf("\n");
        row--;
    }
}