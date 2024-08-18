# Prompt to welcome the user to the time converter
print("Welcome to VRAJ's Time Converter!")

# Prompt user for the number of seconds to be converted
seconds_input = input("Please enter the number of seconds to be converted: ")
# Casting the input to an integer
total_seconds = int(seconds_input)

# Calculate total days from the seconds provided
days = total_seconds // (24 * 60 * 60)
remainder = total_seconds % (24 * 60 * 60)

# Calculating total hours from the remainder
hours = remainder // (60 * 60)

# Updating remainder
remainder %= (60 * 60)

# Calculating total minutes from the updated remainder
minutes = remainder // 60

# Calculating remaining seconds
seconds = remainder % 60

# Printing the results in the specified format of Days: Hours: Minutes: Seconds
print(f"{total_seconds} seconds is equivalent to {days} days {hours:02}:{minutes:02}:{seconds:02}")
