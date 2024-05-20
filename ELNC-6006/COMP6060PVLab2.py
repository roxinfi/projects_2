# Introducing the Shopping helper
print("Welcome to VRAJ's Shopping Helper!")

# Creating a Promt user for user input
itemname = input("please enter the item name to be purchased : ")

# Creating a Promt user for Item Quantity input
itemQuantityStr = input("please enter the quantity of the item to be purchased : ")
itemQuantity = int(itemQuantityStr)

# Creating a Promt user for Item Price input
itemPriceStr = input("please enter the price of the Single item : ")
itemPrice = float(itemPriceStr)

# Creating a Promt user for Item Weight input
itemWeightStr = input("please enter the weight of a single item in lbs : ")
itemWeight = float(itemWeightStr)

# Calculating the total price of the item
totalPrice = itemQuantity * itemPrice

# Calculating the total weight of the item
totalWeight = itemQuantity * itemWeight

# Displaying the total price and total weight of the item
print("Your quantity item(s) at $",itemPrice,"will cost $", totalPrice)
print("Your quantity item(s) at ",itemWeight,"lbs each will weight", totalWeight,"lbs")

# End of the program