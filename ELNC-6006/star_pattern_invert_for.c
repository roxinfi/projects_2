#include<stdio.h>
int main(void)
{
    int row;
    int collumn;
    int size;
    size = 5;

    for(row=size; row>0;row--)
    {
        for(collumn=0;collumn<row;collumn++)
        {
            printf("*");
        }
        printf("\n");
    }
}
