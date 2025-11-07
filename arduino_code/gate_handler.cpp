#include <Arduino.h>
#include <ESP32Servo.h>
#include "gate_handler.h"
#include "lcd_handler.h"

// --- Pin Definitions ---
#define ENTRY_SERVO_PIN 12
#define EXIT_SERVO_PIN 14

// --- Constants ---
const long GATE_OPEN_DURATION = 10000; // Keep gate open for 10 seconds
const int GATE_CLOSED_ANGLE = 0;
const int GATE_OPEN_ANGLE = 90;

// --- Module-specific (static) Variables ---
static Servo entryServo;
static Servo exitServo;
static unsigned long entryGateOpenTimer = 0;
static unsigned long exitGateOpenTimer = 0;
static bool isEntryGateOpen = false;
static bool isExitGateOpen = false;

// Initializes the servo motors.
void setupGate() {
  entryServo.attach(ENTRY_SERVO_PIN);
  exitServo.attach(EXIT_SERVO_PIN);
  entryServo.write(GATE_CLOSED_ANGLE); // Ensure gates are closed on startup
  exitServo.write(GATE_CLOSED_ANGLE);
}

// Opens the entry gate and starts its non-blocking timer.
void openEntryGate() {
  Serial.println("Gate Handler: Opening entry gate.");
  entryServo.write(GATE_OPEN_ANGLE);
  isEntryGateOpen = true;
  entryGateOpenTimer = millis(); // Start the timer!
}

// Opens the exit gate and starts its non-blocking timer.
void openExitGate() {
  Serial.println("Gate Handler: Opening exit gate.");
  exitServo.write(GATE_OPEN_ANGLE);
  isExitGateOpen = true;
  exitGateOpenTimer = millis(); // Start the timer!
}

// This function is called continuously from the main loop().
void handleGate() {
  // Check the entry gate timer.
  if (isEntryGateOpen && (millis() - entryGateOpenTimer > GATE_OPEN_DURATION)) {
    Serial.println("Gate Handler: Timer expired. Closing entry gate.");
    entryServo.write(GATE_CLOSED_ANGLE);
    isEntryGateOpen = false;
    displayIdleMessage(); // Reset LCD to idle state
  }

  // Check the exit gate timer.
  if (isExitGateOpen && (millis() - exitGateOpenTimer > GATE_OPEN_DURATION)) {
    Serial.println("Gate Handler: Timer expired. Closing exit gate.");
    exitServo.write(GATE_CLOSED_ANGLE);
    isExitGateOpen = false;
    displayIdleMessage(); // Reset LCD to idle state
  }
}
