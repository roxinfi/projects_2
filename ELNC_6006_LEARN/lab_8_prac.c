#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable: 4996)
#pragma warning(disable: 6031)

#define MAX_BOXES 200
#define MIN_BOXES 50
#define HEIGHT_MM 10.5
#define DENSITY_G_CM3 0.26

// Function to get the valid number of weights to calculate
int get_number_of_boxes() 
{
    int num_boxes;
    while (1) 
    {
        printf("Enter the number of weights to calculate (50 to 200): ");
        scanf("%d", &num_boxes);
        if (num_boxes >= MIN_BOXES && num_boxes <= MAX_BOXES) 
            {
            break;
            }
        else 
            {
            printf("Invalid input. Please enter a number between 50 and 200.\n");
            }
    }
    return num_boxes;
}

// Function to calculate the weight of each box
void calculate_weights(float weights[], int num_boxes) 
{
    for (int i = 0; i < num_boxes; i++) 
        {
        int length_cm = (rand() % 51) + 25;  // Random number between 25 and 75
        int width_cm = length_cm;  // Width is equal to length
        float volume_cm3 = length_cm * width_cm * HEIGHT_MM;  // Volume in cubic cm
        weights[i] = volume_cm3 * DENSITY_G_CM3 / 1000;  // Convert weight to kilograms
        }
}

// Function to calculate the average weight of the boxes
float calculate_average_weight(float weights[], int num_boxes) 
{
    float total_weight = 0;
    for (int i = 0; i < num_boxes; i++) 
        {
        total_weight += weights[i];
        }
    return total_weight / num_boxes;
}

int main() 
{
    srand(time(NULL));  // Seed the random number generator

    float weights[MAX_BOXES];
    int num_boxes = get_number_of_boxes();

    calculate_weights(weights, num_boxes);

    float average_weight = calculate_average_weight(weights, num_boxes);

    printf("The average weight of the boxes is: %.2f kg\n", average_weight);

    return 0;
}

//end of main.c //