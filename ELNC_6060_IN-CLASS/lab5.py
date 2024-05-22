from random import random
numValueStr = input("Please enter the number of values to generate betwwen 1 and 50 : ")
if not numValueStr.isnumeric():
    print("Error: Invalid vallue")
    exit(-1)
numValue = int(numValueStr)

for num in range(numValue):
    randvalue = int(random() * 100)
    print(num+1, ":", randvalue)
