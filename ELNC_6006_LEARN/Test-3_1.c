#include <stdio.h>
#pragma warning(disable : 4996)
int main() 
{
    int grade, tenthsdivision;

    printf("Enter numerical grade: ");
    scanf_s("%d", &grade);
    tenthsdivision = grade / 10;

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
