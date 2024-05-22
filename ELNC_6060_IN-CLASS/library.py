def getValidUserInput(percentage, min_range, max_range):
    while True:
        value = input(percentage)
        if not value.isnumeric():
            print("Value entered was not a number. Exiting…")
            exit(-1)
        numeric_value = int(value)
        if numeric_value < min_range or numeric_value > max_range:
            print(f"Please enter a value between {min_range} and {max_range}.")
        else:
            return numeric_value

def calculateGPA(grade):
    if grade >= 90:
        return 4.2
    elif grade >= 80:
        return 4.0
    elif grade >= 75:
        return 3.5
    elif grade >= 70:
        return 3.0
    elif grade >= 65:
        return 2.5
    elif grade >= 60:
        return 2.0
    elif grade >= 55:
        return 1.5
    elif grade >= 50:
        return 1.0
    else:
        return "FAIL"
    
def add(x,y):
    return int(x) + int(y)