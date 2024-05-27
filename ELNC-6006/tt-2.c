#include<stdio.h>
int main (void)
{
    int xplane, yplane, rows,columns,first,second,third,forth,fifth,sixth,seventh,eighth,slashplanex,slashplaney;

    printf("Enter the co-ordinates of inside the x-plane : ");
    scanf("%d", &xplane);
    while ((xplane<2)||(xplane>15))
    {
        printf("Invalied input, please enter the range agin: ");
        scanf("%d", &xplane);
    }

    printf("Enter the co-ordinates inside 5 to 11 (odd number only): ");
    scanf("%d",&yplane);a
    while ((yplane<5)||(yplane>11)||(yplane%2==0))
    {
        printf("invliad input, please enter the range again: ");
    }

    printf("Targated acquired X-ais: %d and Y-axis : %d", xplane, yplane);
    
    printf("\n");
    printf("\t");
    first = 0;
    while(first<=16)
    {
        printf(" ");
        printf("|c ");
        first++;
    }
    printf(" |");
    printf("\n");
    printf("\t");
    second = 0;
    while(second<=16)
    {printf(" ");
        printf("|o ");
        second++;
    }printf(" |");
    printf("\n");
    printf("\t");
    third = 0;
    while(third<=16)
    {printf(" ");
        printf("|l ");
        third++;
    }printf(" |");
    printf("\n");
    printf("\t");
    forth = 0;
    while(forth<=16)
    {printf(" ");
        printf("|u "); 
        forth++;
    }printf(" |");
    printf("\n");
    printf("\t");
    fifth = 0;
    while(fifth<=16)
    {printf(" ");
        printf("|m ");
        fifth++;
    }printf(" |");
    printf("\n");
    printf("\t");
    sixth = 0;
    while(sixth<=16)
    {printf(" ");
        printf("|n ");
        sixth++;
    } printf(" |");
   printf("\n");
    
    /*printing the*/
    printf("       ");
    columns = 0;
    printf(" ");
    while (columns<=16)
    {printf(" ");
        printf("|%2d",columns);
        columns++;
    } printf(" |");
    printf("\n");
    printf("\t");
   
    
    seventh=0;
    while(seventh<=16)
    {
        printf(" ");
        printf("|..");
        seventh++;
    }printf(" |");
    printf("\n");
   


    rows = 0;
    while (rows<=16) 
    {
        printf("|rows%2d|:",rows);
        columns = 0;
        while (columns<=16)
        {
            if((rows==yplane)&&(columns==xplane))
            {
                printf("| X ");
            }
            else
            {
                printf("| O ");    
            }
            columns++;
        }printf("|");
    
        printf("\n");
        rows++;
    }
    return 0;
}