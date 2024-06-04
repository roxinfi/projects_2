#include <stdio.h>

int main(void) {
    int enter, deposit, withdraw, balance;
    balance = 0;

    printf("Welcome to the Bank of MIT\n");

    while (1) {
        printf("\nPlease select your options:\n");
        printf("'1' : Deposit\n");
        printf("'2' : Withdraw\n");
        printf("'3' : Check Balance\n");
        printf("'4' : Exit\n");
        printf("Choose your option from above: ");
        scanf("%d", &enter);

        if (enter == 1) 
        {
            printf("Please enter the amount you want to deposit: ");
            scanf("%d", &deposit);
            if (deposit > 0) 
            {
                balance += deposit;
                printf("%d amount has been deposited into your account.\n", deposit);
            } 
            else 
            {
                printf("Invalid deposit amount.\n");
            }
        } 
        else if (enter == 2) 
        {
            printf("Please enter the amount you want to withdraw: ");
            scanf("%d", &withdraw);
            if (withdraw > 0 && withdraw <= balance) 
            {
                balance -= withdraw;
                printf("%d amount has been withdrawn from your account.\n", withdraw);
            } 
            else 
            {
                printf("Invalid withdrawal amount or insufficient balance.\n");
            }
        } 
        else if (enter == 3) 
        {
            printf("Your current balance is: %d\n", balance);
        } else if (enter == 4) 
        {
            printf("Thank you for visiting our bank... See you soon!\n");
            break;
        } else 
        {
            printf("Invalid Entry... please re-enter the options\n");
            while (getchar() != '\n'); // Clear the input buffer
        }
    }
    return 0;
}
