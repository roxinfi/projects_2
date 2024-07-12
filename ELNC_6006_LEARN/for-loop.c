#include<stdio.h>
int main(void) 
{
    int size = 5;
    int row, column, inverse;
    for(row = size; row>0; row--) 
    {
        for (column = 0; column < row; column++)
        {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}