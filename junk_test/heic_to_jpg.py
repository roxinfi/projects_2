import os
from PIL import Image
import pillow_heif

def convert_heic_to_jpg(source_directory, destination_directory):
    # Create the destination directory if it doesn't exist
    if not os.path.exists(destination_directory):
        os.makedirs(destination_directory)

    # List all files in the source directory
    for filename in os.listdir(source_directory):
        if filename.lower().endswith(".heic"):  # Case-insensitive check for HEIC files
            heic_path = os.path.join(source_directory, filename)
            jpg_filename = f"{os.path.splitext(filename)[0]}.jpg"
            jpg_path = os.path.join(destination_directory, jpg_filename)

            # Read HEIC file and convert to JPG
            heif_file = pillow_heif.open_heif(heic_path)
            image = Image.frombytes(
                heif_file.mode,
                heif_file.size,
                heif_file.data,
                "raw",
                heif_file.mode,
                heif_file.stride,
            )

            # Save as JPG in the destination directory
            image.save(jpg_path, "JPEG")
            print(f"Converted {filename} to {jpg_path}")

if __name__ == "__main__":
    # Define source and destination directories
    source_directory = r"C:\Users\vrajp\Downloads\mov_img"
    destination_directory = r"C:\Users\vrajp\Downloads\mov_img_1"

    # Convert HEIC files to JPG
    convert_heic_to_jpg(source_directory, destination_directory)
