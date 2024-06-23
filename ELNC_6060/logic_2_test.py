# COMP6060PVProject1

                                                                # Define arrays globally
resistance_digits = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"]
multipliers = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White", "Gold", "Silver"]
multiplier_values = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 0.1, 0.01]
tolerances = ["Brown", "Red", "Green", "Blue", "Violet", "Grey", "Gold", "Silver"]
tolerance_values = [1, 2, 0.5, 0.25, 0.1, 0.05, 5, 10]
temperature_coefficients = ["Brown", "Red", "Orange", "Yellow", "Blue", "Violet"]
coefficient_values = [100, 50, 15, 25, 10, 5]

                                                                # Function to read an integer value from the user and perform validation
def read_integer(prompt, min_range, max_range):                 # Parameters: prompt string, minimum range value, maximum range value
    while True:                                                 # Loop until a valid integer is entered
        value = input(prompt).strip()
        if value.lower() == 'q':
            return value
        if value.isdigit() and min_range <= int(value) <= max_range:
            return int(value)
        else:
            print("Invalid input. Please enter a valid integer within the specified range or 'q' to exit.")

                                                                # Function to determine the resistance value
def get_resistance_value(num_bands):                            # Parameters: the number of resistance digit bands (2 or 3)

    resistance_value = ""                                       # Initialize an empty string to store the resistance value
    for i in range(num_bands):                                  # Loop through the number of resistance digit bands
        print("Select the resistance band color", i + 1, ":")
        for j in range(len(resistance_digits)):                 # Print the menu of resistance band colors
            print(str(j + 1) + ".", resistance_digits[j])
        digit = read_integer("", 1, 10)
        print("\n")
        if digit == 'q':
            return digit
        resistance_value += str(digit - 1)                      # Store the index as string (0-9)
    return int(resistance_value)

                                                                # Function to determine the resistance multiplier value
def get_multiplier_value():                                     # Parameters: none

    print("Select the multiplier band color:")
    for i in range(len(multipliers)):                           # Print the menu of multiplier band colors
        print(str(i + 1) + ".", multipliers[i])
    multiplier_index = read_integer("", 1, 12)
    print("\n")
    if multiplier_index == 'q':
        return multiplier_index
    return multiplier_values[multiplier_index - 1]

def get_tolerance_value():                                     # Function to determine the resistor tolerance value
    print("Select the tolerance band color:")
    for i in range(len(tolerances)):                           # Print the menu of tolerance band colors
        print(str(i + 1) + ".", tolerances[i])
    tolerance_index = read_integer("", 1, 8)                   # Read the user input
    print("\n")
    if tolerance_index == 'q':
        return tolerance_index
    return tolerance_values[tolerance_index - 1]               # Return the tolerance value

def get_temperature_coefficient_value():                       # Function to determine the resistor temperature coefficient value

    print("Select the temperature coefficient band color:")
    for i in range(len(temperature_coefficients)):             # Print the menu of temperature coefficient band colors
        print(str(i + 1) + ".", temperature_coefficients[i])
    coefficient_index = read_integer("", 1, 6)                 # Read the user input
    print("\n")
    if coefficient_index == 'q':
        return coefficient_index
    return coefficient_values[coefficient_index - 1]

def main():                                                    # Main function to run the program

    print("Welcome to the Resistor Value Calculator!")

    while True:                                                # Loop until the user exits the program
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

        if num_bands == 6:
            temperature_coefficient_value = get_temperature_coefficient_value()
            if temperature_coefficient_value == 'q':
                print("Exiting the program. Goodbye!")
                break
            print("Resistance Value:", resistance_value * multiplier_value, "ohms",
                  "±", tolerance_value, "%",
                  " ", temperature_coefficient_value, "ppm/K")
        else:
            print("Resistance Value:", resistance_value * multiplier_value, "ohms",
                  "±", tolerance_value, "%")

if __name__ == "__main__":
    main()
