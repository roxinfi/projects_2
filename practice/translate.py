from googletrans import Translator

def translate_to_french(text):
    translator = Translator()
    translation = translator.translate(text, src='en', dest='fr')
    return translation.text

def main():
    print("English to French Translator")
    while True:
        input_text = input("Enter text in English (or 'exit' to quit): ")
        if input_text.lower() == 'exit':
            break
        translated_text = translate_to_french(input_text)
        print("Translation in French:", translated_text)
        print()

if __name__ == "__main__":
    main()
