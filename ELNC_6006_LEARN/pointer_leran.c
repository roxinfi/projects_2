#include <stdio.h>

int main() {
    int a = 10;
    int *p;
    p = &a;
    
    printf("Value of a: %d\n", a);                  // Outputs: Value of a: 10
    printf("Address of a: %p\n", &a);        // Outputs the address of a
    printf("Value of p: %p\n", p);           // Outputs the address stored in p (which is the address of a)
    printf("Address of p: %p\n", &p);        // Outputs the address of pointer p
    printf("Value of *p: %d\n", *p);                // Outputs: Value pointed to by p: 10
    printf("Address of p: %p\n", &*p);        // Outputs the address of pointer p
    printf("The value of *p + 10 is %p + %p = %p\n", *p, a , (*p + 10)); // Outputs the value of *p + 10
    // Correct format specifiers for integers
    printf("The sum of a and *p: %p and %p is %p\n", a, (void*)&p, a + *p);

    return 0;
}
