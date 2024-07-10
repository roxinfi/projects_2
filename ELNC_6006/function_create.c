#include<stdio.h>
#include<stdlib.h>
int square(int x)
{
    return x*x;
}

int main()

{
    system("clear");
    int a;
    printf("Enter a number : ");
    scanf("%d", &a);
    printf("The value of a  before call is : %d\n", a);
    printf("The value of a after call is : %d\n", square(a));
    return 0;

}