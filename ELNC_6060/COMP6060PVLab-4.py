# printing the welcome message
print("Welcome to VRAJ's GPA calculator")

# Function to get valid user input
def getValidUserInput(string, minimumrange, maximumrange): 
    while True: # Loop until a valid input is entered
        userinput = input(string)
        if not userinput.isnumeric() : # Check if the input is a number
            print("Value entered was not a number. Exiting...")
            exit()
        value = int(userinput)
        if value < minimumrange or value > maximumrange: # Check if the input is within the range
            print(f"Please enter a value between {minimumrange} and {maximumrange}")
        else: # If the input is valid, return the value
            return value

# Get the user's grades
usergpa = getValidUserInput("Enter your grades: ", 0, 100)

# Calculate the GPA based on the user's grades
if 90<= usergpa <= 100:
    print("Your GPA is 4.2")
elif 80<= usergpa <= 89:
    print("Your GPA is 4.0")
elif 75<= usergpa <= 79:
    print("Your GPA is 3.5")
elif 70<= usergpa <= 74:
    print("Your GPA is 3.0")
elif 65<= usergpa <= 69:
    print("Your GPA is 2.5")
elif 60<= usergpa <= 64:
    print("Your GPA is 2.0")
elif 55<= usergpa <= 59:
    print("Your GPA is 1.5")
elif 50<= usergpa <= 54:
    print("Your GPA is 1.0")
else:
    print("You have failed this course")