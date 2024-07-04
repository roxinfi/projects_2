# Define the file path
file_path = 'C:/Users/vrajp/projects/my_project_2/ELNC_6060/data(lab8).txt'

print("Welcome to VRAJ's sensor data reader!")

# Initialize the variables
tempSum = 0
tempNum = 0
humidSum = 0
humidNum = 0

# Open the file in reading mode
with open(file_path, 'r') as file:
    # Use a for loop to iterate over all the lines in the file
    for line in file:
        # Split the line into the 3 parts separated by 2 tabs
        entry = line.strip().split('\t\t')
        
        # Ensure that the entry has exactly 3 parts
        if len(entry) >= 3:
            timestamp = int(entry[0])
            sensor_name = entry[1]
            sensor_reading = float(entry[2])
            
            # Check if the entry is a temperature or humidity sensor reading
            if sensor_name == 'Temp':
                # Increment the variable tempNum and add to tempSum
                tempNum += 1
                tempSum += sensor_reading
            elif sensor_name == 'Humid':
                # Increment the variable humidNum and add to humidSum
                humidNum += 1
                humidSum += sensor_reading
            else:
                log = f"{sensor_name} is not supported"
                print(log)

# Calculate the averages
tempAvg = tempSum / tempNum 
humidAvg = humidSum / humidNum

# Print the results in the specified format
header = f"{'Temperature Average':^20} | {'Humidity Average':^20}"
values = f"{tempAvg:^20.3f} | {humidAvg:^20.3f}"

print(header)
print(values)
