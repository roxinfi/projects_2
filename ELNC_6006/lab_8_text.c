#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DENSITY_G_CM3 0.26
#define HEIGHT_CM 10.5
#define MAX_BOXES 200
#define MIN_BOXES 50

int main() 
{
    srand(time(NULL));  

    float weights[MAX_BOXES];
    int number_of_boxes;
    float total_weight = 0;

    // number of boxes
    while (1) {
        printf("Enter the number of boxes between (50 to 200): ");
        scanf("%d", &number_of_boxes);
        if (number_of_boxes >= MIN_BOXES && number_of_boxes <= MAX_BOXES) {
            break;
        } else {
            printf("Invalid number of boxes. Please enter a number between 50 and 200.\n");
        }
    }

    // Calculate weights
    for (int i = 0; i < number_of_boxes; i++) {
        int length_cm = (rand() % 51) + 25; 
        int width_cm = length_cm;           
        float volume_cm3 = length_cm * width_cm * HEIGHT_CM; 
        weights[i] = volume_cm3 * DENSITY_G_CM3 / 1000;      
    }

    for (int i = 0; i < number_of_boxes; i++) {
        total_weight += weights[i];
    }

    printf("The average weight of the boxes is %.2f kg.\n", total_weight / number_of_boxes);

    return 0;
}
