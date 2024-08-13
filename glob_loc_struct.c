#include <stdio.h>

struct student {
    char name[20];
    int roll;
    float marks;
};

int avg

void main() {
    struct student class[2]; // Array to store information for 2 students

    for (int i = 0; i < 2; i++) {
        printf("Enter the name of student %d: ", i + 1);
        scanf("%19s", class[i].name); // %19s to avoid buffer overflow
        printf("Enter the roll number of student %d: ", i + 1);
        scanf("%d", &class[i].roll);
        printf("Enter the marks of student %d: ", i + 1);
        scanf("%f", &class[i].marks);
    }

    printf("\nEntered details of students:\n");
    for (int i = 0; i < 2; i++) { // Use 2 here, same as the number of students
        printf("Student %d: Name: %s, Roll Number: %d, Marks: %.2f\n", i + 1, class[i].name, class[i].roll, class[i].marks);
    }

    return 0;
}
