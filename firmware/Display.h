#ifndef DISPLAY_H
#define DISPLAY_H

// ==================================================
// OLED DISPLAY
// ==================================================
//
// This file declares the functions that draw information on
// the 0.96" 128x64 SSD1306-compatible I2C OLED display using the low-RAM SSD1306Ascii driver.
//
// Hardware detection/initialization belongs to DeviceManager.
// Measurement calculations belong to Measurements.
// ==================================================

// Shows the "Power Meter / Starting..." screen during setup().
void showStartupScreen();

// Displays voltage, current, power and accumulated energy.
void displayData();

#endif
