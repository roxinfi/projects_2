# Welcome message personalized with your name
print("Welcome to VRAJ’s temperature converter!")

# Prompt the user for a temperature in Celsius
tempCelsiusStr = input("Please enter a temperature value in Celsius: ")

# Convert the input string to a float
tempCelsius = float(tempCelsiusStr)

# Convert the Celsius temperature to Fahrenheit using the formula
tempFahrenheit = (tempCelsius * 9.0/5.0) + 32

# Create a formatted string to display the result
resultStr = f"{tempCelsius} Celsius = {tempFahrenheit:.2f} Fahrenheit"

# Print the result string
print(resultStr)

# Thank the user for using the program
print("Thanks for using the Temperature Converter program!")
