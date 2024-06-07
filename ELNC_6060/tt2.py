from random import random

# This program generates a random number between 1 and 100
print("Welcome to VRAJ's random number generator")

# Function to get valid user input
def getValidUserInput(string, minimumrange, maximumrange): 
    # Loop until a valid input is entered
    while True: 
        if not string.isnumeric():  # Check if the input is a number
            print("Value entered was not a number. Exiting...")
            exit()
        
        num = int(string)
        # Check if the input is within the range
        if num < minimumrange or num > maximumrange: 
            print("Error: invalid input")
            string = input("Enter the number of values to generate between 1 and 50: ")
              # Prompt user again for input
        # If the input is valid, generate the random numbers, print them and exit the loop
        else:
            return num

# Get the number of values to generate
numValueStr = input("Enter the number of values to generate between 1 and 50: ")
numValue = int(numValueStr)

call = getValidUserInput(numValueStr, 1, 50)

# Generate and print the random numbers

for i in range(call):
    randomVal = int(random() * 100) + 1  # Generates a number between 1 and 100
    print(f'{i + 1} : {randomVal}')
