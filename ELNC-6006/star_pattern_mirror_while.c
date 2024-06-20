#include <stdio.h>

int main(void) {
    int row;
    int column;
    int size;
    int space;

    size = 5; // You can change this value to any desired size

    row = 0;

    while (row < size) 
    {
        space = size - (row) - 1;
        while (space > 0) 
        {
            printf(" ");
            space--;
        }

        column = 0;
        while (column <= row) 
        {
            printf("*");
            column++;
        }

        printf("\n");
        row++;
    }

    return 0;
}
