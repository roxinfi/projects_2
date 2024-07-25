#include<stdio.h>
#include<stdlib.h>

struct tank_t
{
    /// @brief here is my learn by @roxinfi
    int High_lvl; 
    int Low_lvl;
    int Current_lvl;
};

union bank_t
{
    int High_lvl_1;
    int Low_lvl_1;
    int Current_lvl_1;
};

int main()
{
    struct tank_t tank1;
    struct tank_t *tank_p = &tank1;
    union bank_t bank1;

    printf("Size of tank1: %lu\n", sizeof(tank1));
    printf("Size of tank1.High_lvl: %lu\n", sizeof(tank_p->High_lvl));
    printf("Size of tank1.Low_lvl: %lu\n", sizeof(tank_p->Low_lvl));
    printf("Size of tank1.Current_lvl: %lu\n", sizeof(tank_p->Current_lvl));
    printf("\n");
    printf("Size of bank1: %lu\n", sizeof(bank1));
    printf("Size of bank1.High_lvl_1: %lu\n", sizeof(bank1.High_lvl_1));
    printf("Size of bank1.Low_lvl_1: %lu\n", sizeof(bank1.Low_lvl_1));
    printf("Size of bank1.Current_lvl_1: %lu\n", sizeof(bank1.Current_lvl_1));

    printf("Enter the value of tank1.High_lvl: ");
    scanf("%d", &tank1.High_lvl);
    printf("The value of tank1.High_lvl is: %d\n", tank_p->High_lvl);
    printf("\n");

    printf("Enter the value of bank1.High_lvl_1: ");
    scanf("%d", &bank1.High_lvl_1);
    printf("The value of bank1.High_lvl_1 : %d, bank1.Low_lvl_1 : %d, and bank1.Current_lvl_1 : %d\n", bank1.High_lvl_1, bank1.Low_lvl_1, bank1.Current_lvl_1);

    return 0;
}

// Output:
// Size of tank1: 12
// Size of tank1.High_lvl: 4
// Size of tank1.Low_lvl: 4
// Size of tank1.Current_lvl: 4

// Size of bank1: 4
// Size of bank1.High_lvl_1: 4
// Size of bank1.Low_lvl_1: 4
// Size of bank1.Current_lvl_1: 4

// Enter the value of tank1.High_lvl: 10
// The value of tank1.High_lvl is: 10

// Enter the value of bank1.High_lvl_1: 20
// The value of bank1.High_lvl_1 : 20, bank1.Low_lvl_1 : 20, and bank1.Current_lvl_1 : 20