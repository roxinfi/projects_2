from os import path

# Print welcome message
print("Welcome to VRAJ's file search utility!")

# Prompt the user for a file path to open
file_path = input("Enter the file path to search: ")

# Check if the file path provided by the user exists in the filesystem
if not path.exists(file_path):
    print(f"The file {file_path} does not exist… exiting")
    exit()

# Open the file in read mode
file_to_search = open(file_path, 'r')

# Prompt the user for a single word to search
word_to_find = input("Enter a single word (no spaces) to search the file: ")

# Initialize counter
counter = 0

# Read the file word by word and count the occurrences of the word
for line in file_to_search:
    words = line.split()
    for word in words:
        if word == word_to_find:
            counter += 1

# Close the file
file_to_search.close()

print(f"{word_to_find} was found in {file_path}: {counter} times")