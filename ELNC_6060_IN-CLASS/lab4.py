from lib import getValidUserInput, calculateGPA
import sys
import os
import matplotlib
import numpy as np
# Replace 'Your Name' with your actual name
print("Welcome to Your Name’s GPA calculator!")

# Call the function to get user input
grade_percentage = getValidUserInput("Please enter your grade from 0 to 100: ", 0, 100)

# Calculate the GPA based on the grade
gpa = calculateGPA(grade_percentage)

# Print the result
if gpa == "FAIL":
    print("You have failed this course")
else:
    print(f"Your GPA is {gpa}")
