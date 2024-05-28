from os import path

print("welcome to VRAJ's file search utility !")

file_path = input("Enter the file path to search: ")

if not path.exists(file_path):
    print(f'The file {file_path} does not exist...exiting')
    exit()

file_to_search = open(file_path, 'r')

word_to_find = input("Enter a single word (no spaces) to search the file")

counter = 0
if word_to_find == '':
    print("No word entered...exiting")
    exit()

# to be continued...