#include<stdio.h>
#include<conio.h>
int main(void)
{
    int row, column, size;
    printf("Enter the size of the triangle: ");
    scanf("%d", &size);

    row = 0;
    while (row < size)
    {
        column = 0;
        while (column <= row)
        {
            printf("*");
            column++;
        }
        printf("\n");
        row++;
    }

}