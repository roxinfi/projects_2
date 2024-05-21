print("wlecome to VRAJ Temprature converter")

# tae the user input in celcius
tempCelciusstr = float(input("Enter the temprature in Celcius: "))

# Convert Celcius to Fahrenheit
tempFahrenheit = (tempCelciusstr * 9.0/5.0) + 32

# Display the result
print(tempCelciusstr," Celcius"," = ", tempFahrenheit, "Fahrenheit")