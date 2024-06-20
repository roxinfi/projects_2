#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    size = 5;

    row = size;
    while(row>0)
    {
        collumn=0;
        while (collumn<row)
        {
            printf("*");
            collumn++;
        }
        printf("\n");
        
        row--;
    }
}