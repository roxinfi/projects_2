from random import random
# This program generates a random number between 1 and 100
print("Welcome to VRAJ's random number generator")

# Get the number of values to generate
numValueStr = input("Enter the number of values to generate between 1 and 50: ")
numValue = int(numValueStr)

# Function to get valid user input
def getValidUserInput(string, minimumrange, maximumrange):
        
        #  Loop until a valid input is entered
        while True:
            if not string.isnumeric() : # Check if the input is a number
                print("Value entered was not a number. Exiting...")
                exit()
                
            num = int(string)
            # Check if the input is within the range
            if num < minimumrange or num > maximumrange: 
                print(f"Please enter a value between {minimumrange} and {maximumrange}")
                string = input("Enter the number of values to generate between 1 and 50: ")
            # If the input is valid, generate the random numbers, print them and exit the loop

            else:
                for i in range(0, num):
                    randomVal = int(random() * 100)
                    print(f'{i+1} : {randomVal}')
                break

# Call the function to get valid user input
getValidUserInput(numValueStr, 1, 50)