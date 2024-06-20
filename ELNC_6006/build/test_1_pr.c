#include<stdio.h>
int main(void)
{
    int x;
     x = 0x10;
    while(x<=0x40)
    {
        printf("0x%x",x);
        x++;

        if (x%16 == 0)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
        
    }
    return 0;
}