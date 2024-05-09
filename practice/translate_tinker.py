import tkinter as tk
from tkinter import messagebox
from googletrans import Translator

def translate_to_french():
    input_text = text_input.get("1.0", "end-1c")  # Get text from input box
    if input_text.strip() == "":
        messagebox.showwarning("Warning", "Please enter some text.")
        return
    translator = Translator()
    translation = translator.translate(input_text, src='en', dest='fr')
    translated_text.delete("1.0", "end")  # Clear previous translation
    translated_text.insert("end", translation.text)

def clear_text():
    text_input.delete("1.0", "end")  # Clear input text
    translated_text.delete("1.0", "end")  # Clear translated text

# Create main window
root = tk.Tk()
root.title("English to French Translator")

# Create input text box
text_input = tk.Text(root, height=10, width=40)
text_input.pack(pady=10)

# Create translate button
translate_button = tk.Button(root, text="Translate", command=translate_to_french)
translate_button.pack()

# Create clear button
clear_button = tk.Button(root, text="Clear", command=clear_text)
clear_button.pack()

# Create translated text box
translated_text = tk.Text(root, height=10, width=40)
translated_text.pack(pady=10)

# Run the GUI application
root.mainloop()
