# Create 4 arrays that hold the following information:
# a. Resistance digits as string values in order from 0 to 9
# b. Multiplier values as float values in order from 0.01 to 10,000,000,000
# c. Tolerance values as float values in order from 0.1 to 0.05
# d. Temperature coefficient values as integer values in order from 100 to 5

# 1. Print a descriptive welcome message
# 2. The program should run indefinitely until the user enters the character ‘q’
# 3. Prompt the user for the number of bands on the resistor (4-6) or ‘q’ to exit the program.
# a. If the input is a ‘q’, exit the program.
# b. If the input is not a valid number, print an error message and re-prompt for the band number.
# c. If the input is not between the range of 4-6, print an error message and re-prompt for the band number


# Once a valid band number has been entered, prompt the user for the appropriate resistor values
# a. If the number of bands is a 4, prompt the user for 2 resistance digits, 1 multiplier value, and 1 tolerance value
# b. If the number of bands is a 5, prompt the user for 3 resistance digits, 1 multiplier value, and 1 tolerance value
# c. If the number of bands is a 6, prompt the user for 3 resistance digits, 1 multiplier value, 1 tolerance value, and 1 temperature coefficient value.


# 5. Ensure that all resistor values are validated to be numeric, within range, and
# To prompt the user for band colors, a menu must be printed out to the user.
# 1. Depending on the value being prompted, print a numbered menu listing the valid band colors for that value. For example, if prompting for the resistance digits, 
# the following menu should be printed: 
# 1. Black 2. Brown 3. Red 4. Orange 5. Yellow 6. Green 7. Blue 8. Violet 9. Grey 10. White
# 2. Accept a value from the user, and validate that it is a numeric value within the range of the numbered list printed. For example, if prompting for the resistance digits, the input should be between 1 and 10
# 3. Retrieve the equivalent value from the appropriate array. For example, if prompting for the resistance digits, an input of 7 should get the digit value ‘6’.

# To build the resistance value from the different digits, the digits are first concatenated together in string form. When the user is done selecting all the bands for resistance digits, the string should be cast to an integer.
# This value can then be used to multiply it with the multiplier value selected by the user to determine the resistor’s resistance value.
# The tolerance and temperature coefficients can be used as string values.
# PYTHON FILE STRUCTURE
# 1. Comments must be used to explain the program’s functionality.
# 2. Variable and function naming conventions must be consistent (camel-case, snake-case, …)
# 3. The following functions must be created:
# a. A function to read an integer value from the user and perform validation to ensure the value entered by the user is a number, and within the range specified
# i. Parameters: prompt string, minimum range value, maximum range value
# ii. Return: a valid integer value within the min-max range
# b. A function to determine the resistance value. This function will repeatedly use function (a) to prompt the user for the band color, then find the correct resistance digit values from the array created at the beginning
# i. Parameters: the number of resistance digit bands (2 or 3)
# ii. Return: the resistance value that corresponds to the user’s choice
# c. A function to determine the resistance value. This function will use function (a) to prompt the user for the band color, then find the correct resistance multiplier value from the array created at the beginning
# i. Parameters: none
# ii. Return: the resistance multiplier value that corresponds to the user’s choice
# d. A function to determine the resistor tolerance value. This function will use function (a) to prompt the user for the band color, then find the correct resistor tolerance value from the array created at the beginning
# i. Parameters: none
# ii. Return: the resistor tolerance value that corresponds to the user’s choice
# e. A function to determine the resistor temperature coefficient value. This function will use function (a) to prompt the user for the band color, then find the correct resistor temperature coefficient value from the array created at the beginning
# i. Parameters: none
# ii. Return: the resistor temperature coefficient value that corresponds to the user’s choice


