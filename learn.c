#include<stdio.h>

void main()
{
    int k = 8;
    int m = 7;
    k<m ? k++ : m=k;
    printf("%d \n", k);
    return 0;
}