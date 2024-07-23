#include<stdio.h>

void sum(int *a, int *b, int *c)
{
     *c = *a + *b;
}

int main()
{
    int a =15;
    int b = 20;
    int c = 0;
    printf("Before c : %d\n", c);
    sum(&a,&b,&c);
    printf("After c : %d", c);

}