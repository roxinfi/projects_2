import os

print('hello world')
Name = str(input("Please enter your name: "))


def words():
    string = len(Name)
    return print("the total number of characters in string", Name, "is", string)


words()