import math

# Print welcome message
print("Welcome to VRAJ's number analyzer!")

# Creating the dataset
dataset = { "radii": [5.5, 6.3],
            "angles": [56, 180, 320, 15, 90]
          }

# Prompt user for number set to analyze
print("Select a set of numbers to analyze:")

# Print the number group menu to select from
counter = 1
for key in dataset.keys():
    print(f"{counter}: {key}")
    counter += 1

# Read input from user from the menu
keyNum = int(input())

# Function to calculate and print circle area and sphere volume
def analyze_radii(radii):
    for radius in radii:
        area = math.pi * radius ** 2
        volume = (4 / 3) * math.pi * radius ** 3
        print(f"Circle with radius {radius} has area {area:.2f}")
        print(f"Sphere with radius {radius} has volume {volume:.2f}")

# Function to calculate and print sine and cosine of angles
def analyze_angles(angles):
    for angle in angles:
        sin_val = math.sin(math.radians(angle))
        cos_val = math.cos(math.radians(angle))
        print(f"Angle {angle}° has sine {sin_val:.2f}")
        print(f"Angle {angle}° has cosine {cos_val:.2f}")

# Analyze the selected set of numbers
def main():
    if keyNum == 1:
        analyze_radii(dataset["radii"])
    elif keyNum == 2:
        analyze_angles(dataset["angles"])
    else:
        print("Invalid number… exiting")

if __name__ == "__main__":
    main()
