// Write a program that asks the user to enter two different integers, obtains the numbers from the user, then prints the larger number followed by the words "is larger." Use only the single-selection form of the if statement (ie: in other words... you are not allowed to use two if statements).
#include <stdio.h>
int main() {
    int num1, num2;
    printf("Enter two different integers: ");
    scanf("%d %d", &num1, &num2);
    if (num1 > num2) {
        printf("%d is larger.\n", num1);
    } else {
        printf("%d is larger.\n", num2);
    }
    return 0;
}