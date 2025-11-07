#ifndef GATE_HANDLER_H
#define GATE_HANDLER_H

// Initializes the servo motors and sets their starting positions.
void setupGate();

// Opens the entry gate and starts its auto-close timer.
void openEntryGate();

// Opens the exit gate and starts its auto-close timer.
void openExitGate();

// Checks the timers and closes the gates if needed.
// This function is non-blocking and safe to call in the main loop.
void handleGate();

#endif
