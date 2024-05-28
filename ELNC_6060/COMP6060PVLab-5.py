from random import random
print("Welcome to VRAJ's random number generator")

numValueStr = input("Enter the number of values to generate between 1 and 50: ")
numValue = int(numValueStr)
def getValidUserInput(string, minimumrange, maximumrange):
        while True:
            string = numValueStr
            if not string.isnumeric() :
                print("Value entered was not a number. Exiting...")
                exit()
            num = int(string)
            if num < minimumrange or num > maximumrange:
                print(f"Please enter a value between {minimumrange} and {maximumrange}")
            else:
                return num
            


getValidUserInput(numValueStr, 1, 50)

for num in range(0, numValue):
    randomVal = int(random() * 100)
    print(f'{num+1} : {randomVal}')