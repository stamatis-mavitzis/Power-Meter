#ifndef STATUS_LED_H
#define STATUS_LED_H

// ==================================================
// STATUS LEDs
// ==================================================
//
// This file declares all functions related to the three LEDs:
//
// D2 -> overall system status
// D3 -> successful measurement flash
// D4 -> permanent measurement-error indication
// ==================================================

// Configures the three LED pins as outputs and turns them OFF.
void setupStatusLEDs();

// Turns the D2 system LED ON only when INA219 + OLED + SD are available.
void updateSystemStatusLED();

// successful = true  -> flash D3 for 40 ms
// successful = false -> turn D4 ON permanently
void measurementLED(bool successful);

#endif
