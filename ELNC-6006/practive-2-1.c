#include<stdio.h>
int main (void)
{
    int xplane, yplane, rows,columns,first;

    printf("Enter the co-ordinates of inside the x-plane : ");
    scanf("%d", &xplane);
    while ((xplane<2)||(xplane>15))
    {
        printf("Invalied input, please enter the range agin: ");
        scanf("%d", &xplane);
    }

    printf("Enter the co-ordinates inside 5 to 11 (odd number only): ");
    scanf("%d",&yplane);
    while ((yplane<5)||(yplane>11)||(yplane%2==0))
    {
        printf("invliad input, please enter the range again: ");
        scanf("%d",&yplane);
    }

    printf("Targated acquired X-ais: %d and Y-axis : %d", xplane, yplane);
    printf("\n\n");


    printf("   ");
    columns = 1;
    while (columns<=16)
    {
        printf("c\no\nl\nu\nm\ns\n%3d",columns);
        columns++;
    }
    printf("\n");

    rows = 0;
    while (rows<=16) 
    {
        printf("%2d :",rows);
        columns = 1;
        while (columns<=16)
        {
            if((rows==yplane)&&(columns==xplane))
            {
                printf(" X ");
            }
            else
            {
                printf(" O ");    
            }
            columns++;
        }
    
        printf("\n");
        rows++;
    }

    
}