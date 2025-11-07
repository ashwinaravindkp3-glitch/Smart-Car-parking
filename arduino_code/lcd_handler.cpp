#include <Arduino.h>
#include "lcd_handler.h"

// --- LCD Configuration ---
// IMPORTANT: You may need to change the I2C address (0x27 is common)
// and the dimensions (16x2 is common).
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// --- Module-specific (static) Variables ---
// Initialize the LCD object.
static LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Initializes the LCD.
void setupLcd() {
  lcd.init();
  lcd.backlight();
  displayIdleMessage();
}

// Displays the entry message with the given slot number.
void displayEntryMessage(int slotNumber) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please proceed");
  lcd.setCursor(0, 1);
  lcd.print("to slot: " + String(slotNumber));
}

// Displays the exit message.
void displayExitMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("visiting!");
}

// Displays the idle/welcome message.
void displayIdleMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ParkSensei");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
}

// Clears the LCD screen.
void clearLcd() {
  lcd.clear();
}
