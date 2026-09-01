#include "Display.h"

#include <Arduino.h>

#include "DeviceManager.h"
#include "Measurements.h"

// ==================================================
// STARTUP SCREEN
// ==================================================
//
// SSD1306Ascii is intentionally used instead of Adafruit_SSD1306.
// It does not allocate a 1024-byte full-screen framebuffer, which
// leaves enough SRAM for SdFat and the rest of the power-meter code.
// ==================================================

void showStartupScreen() {

  display.clear();
  display.setFont(Adafruit5x7);
  display.set1X();

  display.println(F("Power Meter"));
  display.println(F("Starting..."));
}


// ==================================================
// DISPLAY CURRENT POWER-METER VALUES
// ==================================================
//
// The display is updated as text only. Trailing spaces overwrite any
// characters left from a previous value that had more digits.
// ==================================================

void displayData() {

  display.setCursor(0, 0);
  display.print(F("Voltage: "));
  display.print(loadvoltage, 2);
  display.print(F(" V       "));

  display.setCursor(0, 2);
  display.print(F("Current: "));
  display.print(current_mA, 1);
  display.print(F(" mA      "));

  display.setCursor(0, 4);
  display.print(F("Power:   "));
  display.print(power_mW, 1);
  display.print(F(" mW      "));

  display.setCursor(0, 6);
  display.print(F("Energy:  "));
  display.print(energy_mWh, 3);
  display.print(F(" mWh     "));
}
