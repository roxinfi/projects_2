/*
© - FANSHAWE COLLAGE, LONDON, ONTARIO
ELECTRONIC& EMBEDDED SYSTEM DEVELOPMENT
AUTHOR : VRAJ PATEL(ID : 1243437), DANIYA JACOB(ID : 1205772)
DATE : 2024 - 05 - 31
This program allows user to check the respected grade with help of switch statement
*/
#include <stdio.h>
#pragma warning(disable : 4996)
int main()
    {
        int grade, tenthsdivision;

        printf("Enter numerical grade: ");
        scanf_s("%d", &grade);
        tenthsdivision = grade / 10;

        if ((grade < 0) || (grade > 100))
            {
                printf("Error grade must be in between 0 to 100");
            }
        else
            {
            printf("Entered numerical grade: %d\n", grade);
                switch (tenthsdivision)
                    {
                        case 10:
                        case 9:
                            printf("Letter grade: A\n");
                         break;
                        case 8:
                            printf("Letter grade: B\n");
                        break;
                        case 7:
                            printf("Letter grade: C\n");
                        break;
                        case 6:
                            printf("Letter grade: D\n");
                        break;
                        default:
                            printf("Letter grade: F\n");
                        break;

                    }
            }
    }