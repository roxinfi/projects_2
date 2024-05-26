#include<stdio.h>
int main(void)
{
    int xaxis, yaxis, rows, columns;
    
    printf("Enter the location of the target on the x-axis [in range of 2 to 15]: ");
    scanf("%d", &xaxis);
    while ((xaxis < 2) || (xaxis > 15))
    {
        printf("Invalid input. Please enter the xaxis in range of 2 to 15: ");
        scanf("%d", &rows);
    }
    printf("Enter the location of the target on the y-axis [odd value in range of 5 to 11): ");
    scanf("%d", &yaxis);
    while ((yaxis < 5) || (yaxis > 11) || (yaxis % 2 == 0))
    {
        printf("Invalid input. Please enter the yaxis in range of 5 to 11 and odd value: ");
        scanf("%d", &yaxis);
    }
    printf("Target location: x-axis = %d, y-axis = %d \n\n", xaxis, yaxis);

    rows = 0;
    while (rows <=16)
    {
        columns = 0;
        printf("rows %2d: ", rows);
        while(columns <= 16)
        {
            if (columns == xaxis && rows == yaxis)
            {
                printf("X");
            }
            else
            {
                printf("O");
            }
            columns++;
        }
        printf("\n");
        rows++;
    }
    printf("\n Target Acquired! \n ");
}