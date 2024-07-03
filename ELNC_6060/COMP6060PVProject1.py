# COMP6060PVProject1

# Define arrays globally
resistance_num = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
multiplier_values = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 0.1, 0.01]
tolerance_values = [1, 2, 0.5, 0.25, 0.1, 0.05, 5, 10]
coefficient_values = [100, 50, 15, 25, 10, 5]

# Function to read an integer value from the user and perform validation
def read_integer(prompt, min_range, max_range): # Function to read an integer value from the user and perform validation
    while True: # Loop to keep asking for input until a valid integer within the specified range is entered
        value = input(prompt).strip()
        if value.lower() == 'q': # Check if the user wants to exit the program
            return value
        if value.isdigit() and min_range <= int(value) <= max_range:
            return int(value)
        else: # Print an error message if the input is invalid
            print("Invalid input. Please enter a valid integer within the specified range or 'q' to exit.")

# Function to determine the resistance value
def get_resistance_value(num_bands): # Function to determine the resistance value
    resistance_digits = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"]
    resistance_value = ""
    for i in range(num_bands): # Loop to get the resistance band colors from the user
        print("Select the resistance band color", i + 1, ":")
        for j in range(len(resistance_digits)): # Loop to display the resistance band color options
            print(str(j + 1) + ".", resistance_digits[j])
        digit = read_integer("", 1, 10) # Read the user input for the resistance band color
        print("\n")
        if digit == 'q': # Check if the user wants to exit the program
            return digit
        resistance_value += str(digit - 1)
    return int(resistance_value) # Return the resistance value as an integer

# Function to determine the resistance multiplier value
def get_multiplier_value(): 
    multipliers = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White", "Gold", "Silver"]
    print("Select the multiplier band color:") # Display the options for the multiplier band colors
    for i in range(len(multipliers)): # Loop to display the multiplier band color options
        print(str(i + 1) + ".", multipliers[i])
    multiplier_index = read_integer("", 1, 12)
    print("\n")
    if multiplier_index == 'q': # Check if the user wants to exit the program
        return multiplier_index
    return multiplier_values[multiplier_index - 1] # Return the multiplier value

# Function to determine the resistor tolerance value
def get_tolerance_value():
    tolerances = ["Brown", "Red", "Green", "Blue", "Violet", "Grey", "Gold", "Silver"]
    print("Select the tolerance band color:")
    for i in range(len(tolerances)): # Loop to display the tolerance band color options
        print(str(i + 1) + ".", tolerances[i]) # Display the tolerance band color options
    tolerance_index = read_integer("", 1, 8)
    print("\n")
    if tolerance_index == 'q':
        return tolerance_index
    return tolerance_values[tolerance_index - 1]    # Return the tolerance value

# Function to determine the resistor temperature coefficient value
def get_temperature_coefficient_value(): 
    temperature_coefficients = ["Brown", "Red", "Orange", "Yellow", "Blue", "Violet"]
    print("Select the temperature coefficient band color:")
    for i in range(len(temperature_coefficients)): # Loop to display the temperature coefficient band color options
        print(str(i + 1) + ".", temperature_coefficients[i])
    coefficient_index = read_integer("", 1, 6)
    print("\n")
    if coefficient_index == 'q': # Check if the user wants to exit the program
        return coefficient_index
    return coefficient_values[coefficient_index - 1]

# Main function to run the resistor value calculator
def main():
    print("Welcome to the Resistor Value Calculator!")

    while True: # Loop to keep the program running until the user chooses to exit
        num_bands = read_integer("Enter the number of bands on the resistor (4-6) or 'q' to exit: ", 4, 6)
        if num_bands == 'q':
            print("Exiting the program. Goodbye!")
            break

        resistance_value = get_resistance_value(2 if num_bands == 4 else 3)
        if resistance_value == 'q':
            print("Exiting the program. Goodbye!")
            break

        multiplier_value = get_multiplier_value()
        if multiplier_value == 'q':
            print("Exiting the program. Goodbye!")
            break

        tolerance_value = get_tolerance_value()
        if tolerance_value == 'q':
            print("Exiting the program. Goodbye!")
            break

        if num_bands == 6: # Check if the resistor has a temperature coefficient band
            temperature_coefficient_value = get_temperature_coefficient_value()
            if temperature_coefficient_value == 'q':
                print("Exiting the program. Goodbye!")
                break
            print("Resistance Value:", resistance_value * multiplier_value, "ohms",
                  "±", tolerance_value, "%",
                  "", temperature_coefficient_value, "ppm/K")
        else: # Display the resistance value with the specified multiplier and tolerance
            print("Resistance Value:", resistance_value * multiplier_value, "ohms",
                  "±", tolerance_value, "%")

if __name__ == "__main__": # Run the main function if the script is executed
    main()
