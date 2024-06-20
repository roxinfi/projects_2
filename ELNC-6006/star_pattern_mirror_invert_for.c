#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    int space;

    size = 5;
    for(row=size;row>0;row--)
    {
        for(space = size - row;space>0;space--)
        {
            printf(" ");;
        }
        for (collumn = 0; collumn<row; collumn++)
        {
            printf("*");
        }
        printf("\n");
        
    }
}