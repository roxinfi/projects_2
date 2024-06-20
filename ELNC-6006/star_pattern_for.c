#include<stdio.h>
int main(void)
{
    int collumns;
    int row;
    int size;
    size = 5;

    for(row=1;row<=size;row++)
    {
        for(collumns=0;collumns<row;collumns++)
        {
            printf("*");
        }
        printf("\n");
    }
}