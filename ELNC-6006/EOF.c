#include<stdio.h>
int main(void)
{
    int enter,deposit,withdraw,check,exit, balance;
    balance = 0;
    check = balance;
    printf("Welcome to the Bank of MIT\n");
    
    while(1)
    {
        printf("please select your options:\n'1' : Deposit\n'2' : Withdraw\n'3' : check\n'4' : exit\nChoose your option from above: ");
        scanf("%d", &enter);
        {
            if (enter == 1)
            {
                printf("Please enter the amount you want to deposit : ");
                scanf("%d", &deposit);
                printf("%d amount is been deposited inside you account\n\n", deposit);
                balance = balance + deposit;
                exit;
            }
            else if (enter == 2)
            {
                printf("Please enter the amount you want to withdraw : ");
                scanf("%d", &withdraw);
                printf("%d amount is been withdrawn from your account \n\n", withdraw);
                balance = balance - withdraw;
                exit;
            }
            else if (enter == 3)
            {
                printf("Your current balance is %d : \n\n", check);
            }
            else if (enter == 4)
            {
                printf("Thank you for visiting our bank... See you Soon");
                break;
            }
            else
            {
                printf("Invalid Entry... please re-enter the options");
                printf("\n\n");
                while (getchar() != '\n');
            }
            
        }
    }
    return 0;
}