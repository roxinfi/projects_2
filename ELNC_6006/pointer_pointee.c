#include <stdio.h>
int main(void)
{

int var = 10;  // var is the pontee
int *ptr = &var;  // ptr is the pointer to the pontee var
int sac = 10;
printf("Value of var: %d\n", var);         // Direct access to the pontee
printf("Address of var: %p\n", (void *)&var);  // Address of the pontee
printf("Value of ptr: %p\n", (void *)ptr); // Pointer's value (address of the pontee)
printf("Dereferenced value of ptr: %d\n", *ptr);  // Value of the pontee via the pointer
printf("The address of sac is : %p\n", (void *)&sac);

}