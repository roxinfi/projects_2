#include <LiquidCrystal_I2C.h>

// Set the LCD address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    // Initialize the LCD
    lcd.begin(16, 2);
    lcd.backlight();
    
    // Print "Hello, World!" on the LCD
    lcd.print("Hello, World!");
}

void loop() {
    // Nothing to do here
}