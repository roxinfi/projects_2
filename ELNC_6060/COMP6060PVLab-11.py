import os

def main():
    # Print welcome message
    print("\nWelcome to VRAJ's folder analyzer program!\n")

    # Prompt the user for a path
    user_path = input("Enter a valid folder path to be analyzed: ")
    print("\n")
    # Validate the path
    if not os.path.exists(user_path):
        print(f"Error: The path {user_path} does not exist")
        return

    if not os.path.isdir(user_path):
        print(f"Error: The path {user_path} is not a folder")
        return

    # Prompt the user for a file size limit
    file_size_limit_str = input("Enter a file size in bytes: ")
    print("\n")

    # Validate the file size limit
    if not file_size_limit_str.isdigit():
        print("Error: The value is not a number.")
        return

    file_size_limit = int(file_size_limit_str)

    # Print the current working directory
    print(f"Current working directory: {os.getcwd()}")

    # Change the current working directory to user_path
    os.chdir(user_path)
    print(f"Changed current working directory: {os.getcwd()}")

    # Initialize variables
    folder_count = 0
    file_count = 0
    full_size = 0
    file_limit_count = 0

    # Iterate through the user path
    for entry in os.scandir('.'):
        if entry.is_dir():
            folder_count += 1
        elif entry.is_file():
            file_count += 1
            entry_size_bytes = os.path.getsize(entry.path)
            full_size += entry_size_bytes
            if entry_size_bytes >= file_size_limit:
                file_limit_count += 1

    # Print the results
    print(f"Files found in {user_path}: {file_count}\n")
    print(f"Folders found in {user_path}: {folder_count}\n")
    print(f"All files in {user_path} take up: {full_size} bytes\n")
    print(f"Files in {user_path} that exceeded {file_size_limit} bytes: {file_limit_count}\n")

if __name__ == "__main__":
    main()
