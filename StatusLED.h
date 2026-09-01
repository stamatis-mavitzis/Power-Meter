#ifndef STATUS_LED_H
#define STATUS_LED_H

// ==================================================
// STATUS LEDs
// ==================================================
//
// D2 -> latched System OK status for INA219 + SD card
// D3 -> successful measurement flash
// D4 -> latched error indication
//
// D2 turns OFF permanently for the current boot session if either
// INA219 or SD becomes unavailable.
//
// D4 turns ON permanently for the current boot session if:
//   - a measurement is invalid/failed
//   - INA219 is unavailable
//   - SD card is unavailable
// ==================================================

// Configures the three LED pins as outputs and clears the latches
// at startup/reset.
void setupStatusLEDs();

// Updates D2 using ONLY INA219 + SD status.
// Also latches D4 ON if INA219 or SD is unavailable.
void updateSystemStatusLED();

// successful = true  -> flash D3 for 40 ms
// successful = false -> latch D4 ON until reset/power removal
void measurementLED(bool successful);

#endif
