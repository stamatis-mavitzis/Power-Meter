#include "Display.h"

#include <Arduino.h>

#include "DeviceManager.h"
#include "Measurements.h"

// ==================================================
// STARTUP SCREEN
// ==================================================
//
// Called by DeviceManager after the OLED initializes correctly.
// ==================================================

void showStartupScreen() {

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println(F("Power Meter"));
  display.println(F("Starting..."));

  // Send the prepared text buffer to the physical OLED.
  display.display();
}


// ==================================================
// DISPLAY CURRENT POWER-METER VALUES
// ==================================================
//
// This function only formats and draws the current values.
// DeviceManager has already checked that the OLED is connected
// before this function is called.
// ==================================================

void displayData() {

  // Start with an empty display buffer.
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);


  // ==================================================
  // VOLTAGE
  // ==================================================

  display.print(F("Voltage: "));
  display.print(loadvoltage, 2);
  display.println(F(" V"));


  // ==================================================
  // CURRENT
  // ==================================================

  display.print(F("Current: "));
  display.print(current_mA, 1);
  display.println(F(" mA"));


  // ==================================================
  // POWER
  // ==================================================

  display.print(F("Power: "));
  display.print(power_mW, 1);
  display.println(F(" mW"));


  // ==================================================
  // ENERGY
  // ==================================================

  display.print(F("Energy: "));
  display.print(energy_mWh, 3);
  display.println(F(" mWh"));


  // Copy the completed buffer to the physical OLED screen.
  display.display();
}
