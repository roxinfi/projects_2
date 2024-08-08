#COMP6060PVLab-12
import os
import shutil

# NAME with your actual name
print("Welcome to VRAJ's file organizer!")

# Prompt the user for a folder name to organize
path_to_organize = input("Enter a path to a folder to organize: ")

# Verify that the folder exists
if not os.path.exists(path_to_organize):
    print("Error: The folder does not exist.")
    exit()

# Create folders
try:
    os.mkdir(os.path.join(path_to_organize, "Documents"))
except FileExistsError:
    print("Folder exists, not creating...")

try:
    os.mkdir(os.path.join(path_to_organize, "Copied_Images"))
except FileExistsError:
    print("Folder exists, not creating...")

# Scan folder and organize files
for entry in os.scandir(path_to_organize):
    if entry.is_file():
        file_extension = os.path.splitext(entry.path)[1]
        
        if file_extension in [".txt", ".docx"]:
            dest = os.path.join(path_to_organize, "Documents")
            shutil.move(entry.path, dest)
            print(f"Moved {entry.path}")
        elif file_extension == ".pdb":
            os.remove(entry.path)
            print(f"Deleted {entry.path}")
        elif file_extension in [".png", ".jpeg"]:
            dest = os.path.join(path_to_organize, "Copied_Images")
            shutil.copy(entry.path, dest)
            print(f"Copied {entry.path}")
        else:
            print(f"{file_extension} is not supported.")

# END OF PROGRAM
