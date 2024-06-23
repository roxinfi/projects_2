def read_integer(prompt, min_range, max_range):
    while True:
        value = input(prompt).strip()
        if value.lower() == 'q':
            return value
        if value.isdigit() and min_range <= int(value) <= max_range:
            return int(value)
        else:
            print("Invalid input. Please enter a valid integer within the specified range or 'q' to exit.")

def get_resistance_value(num_bands):
    resistance_digits = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"]
    resistance_value = ""
    for i in range(num_bands):
        print("Select the resistance band color", i + 1, ":")
        for j in range(len(resistance_digits)):
            print(str(j + 1) + ".", resistance_digits[j])
        digit = read_integer("", 1, 10)
        if digit == 'q':
            return digit
        resistance_value += str(digit - 1)  # Store the index as string (0-9)
    return int(resistance_value)

def get_multiplier_value():
    multipliers = ["Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White", "Gold", "Silver"]
    multiplier_values = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 0.1, 0.01]
    print("Select the multiplier band color:")
    for i in range(len(multipliers)):
        print(str(i + 1) + ".", multipliers[i])
    multiplier_index = read_integer("", 1, 12)
    if multiplier_index == 'q':
        return multiplier_index
    return multiplier_values[multiplier_index - 1]

def get_tolerance_value():
    tolerances = ["Brown", "Red", "Green", "Blue", "Violet", "Grey", "Gold", "Silver"]
    tolerance_values = [1, 2, 0.5, 0.25, 0.1, 0.05, 5, 10]
    print("Select the tolerance band color:")
    for i in range(len(tolerances)):
        print(str(i + 1) + ".", tolerances[i])
    tolerance_index = read_integer("", 1, 8)
    if tolerance_index == 'q':
        return tolerance_index
    return tolerance_values[tolerance_index - 1]

def get_temperature_coefficient_value():
    temperature_coefficients = ["Brown", "Red", "Orange", "Yellow", "Blue", "Violet"]
    coefficient_values = [100, 50, 15, 25, 10, 5]
    print("Select the temperature coefficient band color:")
    for i in range(len(temperature_coefficients)):
        print(str(i + 1) + ".", temperature_coefficients[i])
    coefficient_index = read_integer("", 1, 6)
    if coefficient_index == 'q':
        return coefficient_index
    return coefficient_values[coefficient_index - 1]

def main():
    print("Welcome to the Resistor Value Calculator!")

    while True:
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
