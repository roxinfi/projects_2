#include <stdio.h>
#include <stdlib.h>

int suare(int a) 
{
    int b = a * a;
    a = b;
    return a;
}

int main() {
    int a = 5;
    printf("before: %d\n", a);
    a = suare(a); // Use the return value to update 'a'
    printf("after: %d\n", a);
    return 0;
}






// #include<stdio.h>
// #include<stdlib.h>

// void square(int *a)
// {
//     int b = (*a) * (*a);
//     *a = b;
// }

// int main()
// {
//     int a = 5;
//     printf("before: %d\n", a);
//     square(&a);
//     printf("after: %d\n", a);
//     return 0;
// }