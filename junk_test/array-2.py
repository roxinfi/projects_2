def place():
    location = int(input("Enter the location: "))
    if location > len(store):
        print("Invalid location")
    elif location < 0:
        print("invalid location")
    else:
        print(store[location])
def loc():
    array = input("Enter the array: ")
    if array.isnumeric() == False:
        return list(array)
    else:
        exit()

store = loc()
place()

