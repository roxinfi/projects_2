#include<stdio.h>

int main(void)
{
    int size;
    printf("Enter the size of the size: ");  // Message can be simplified to "Enter the size:"
    scanf_s("%d", &size);  // Reads the size of the pattern

    printf("\n\n\n");
    
    int row = 0;

    while (row < size)
    {
        int column = 0;  // This initialization needs to be inside the outer loop
        
        while (column < size)
        {
            printf("x");
            column++;
        }

        printf("\n");
        row++;
    }
    return 0;
}
