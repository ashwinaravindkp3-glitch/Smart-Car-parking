#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H

#include <LiquidCrystal_I2C.h>

// Initializes the LCD.
void setupLcd();

// Displays the entry message with the given slot number.
void displayEntryMessage(int slotNumber);

// Displays the exit message.
void displayExitMessage();

// Displays the idle/welcome message.
void displayIdleMessage();

// Clears the LCD screen.
void clearLcd();

#endif
