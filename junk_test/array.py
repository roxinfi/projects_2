from typing import List

def split() -> List[int]:
    array = input("Enter the array: ")
    if array.isnumeric() == False:
        array = array.split()
        store = []
        for i in array:
            store.append(str(i))
        return store
    else:
        return array
    
print(split())