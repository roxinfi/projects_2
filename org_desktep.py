import os
import shutil

def organize_files(source_dir):
    # List of allowed file extensions
    allowed_extensions = ['docx', 'xlsx', 'csv', 'pdf', 'pptx', '7z', 'rar', 'zip']
    # List of image file extensions
    image_extensions = ['jpg', 'jpeg', 'png', 'gif', 'bmp', 'tif', 'tiff']

    # Iterate through all files in the source directory
    for filename in os.listdir(source_dir):
        source_file_path = os.path.join(source_dir, filename)

        # Skip directories
        if os.path.isdir(source_file_path):
            continue

        # Get the file extension
        _, extension = os.path.splitext(filename)

        # Determine the destination directory based on the file extension
        if extension[1:] in allowed_extensions:
            destination_dir = os.path.join(source_dir, "Documents")
        elif extension[1:] in image_extensions:
            destination_dir = os.path.join(source_dir, "Images")
        else:
            continue  # Skip files with extensions not in the allowed list

        # Create the destination directory if it doesn't exist
        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

        # Move the file to the corresponding destination directory
        destination_file_path = os.path.join(destination_dir, filename)
        shutil.move(source_file_path, destination_file_path)
        print(f"Moved {filename} to {destination_dir}")

if __name__ == "__main__":
    source_directory = r"C:\Users\vrajp\Downloads\Organize"
    organize_files(source_directory)
