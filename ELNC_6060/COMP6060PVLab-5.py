from random import random
# This program generates a random number between 1 and 100
print("Welcome to VRAJ's random number generator")

# Get the number of values to generate
numValueStr = input("Enter the number of values to generate between 1 and 50: ")
numValue = int(numValueStr)
def getValidUserInput(string, minimumrange, maximumrange): # Function to get valid user input
        while True: #  Loop until a valid input is entered
            string = numValueStr
            if not string.isnumeric() : # Check if the input is a number
                print("Value entered was not a number. Exiting...")
                exit()
            num = int(string)
            if num < minimumrange or num > maximumrange: # Check if the input is within the range
                print(f"Please enter a value between {minimumrange} and {maximumrange}")
            else: # If the input is valid, generate the random numbers
                for num in range(0, numValue):
                    randomVal = int(random() * 100)
                    print(f'{num+1} : {randomVal}')


getValidUserInput(numValueStr, 1, 50) # Call the function to get valid user input