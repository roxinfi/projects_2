#include<stdio.h>

int main(void)
{
    int a[5];

    for (int loop = 0; loop < 5; loop++)
    {
        printf("Enter a number for a[%d]: ", loop);
        scanf("%d", &a[loop]);
    }

    return 0;
}