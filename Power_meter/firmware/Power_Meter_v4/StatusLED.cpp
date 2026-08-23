#include "StatusLED.h"

#include <Arduino.h>

#include "Config.h"
#include "DeviceManager.h"

// ==================================================
// CONFIGURE LED PINS
// ==================================================

void setupStatusLEDs() {

  pinMode(SYSTEM_OK_LED, OUTPUT);
  pinMode(MEASUREMENT_OK_LED, OUTPUT);
  pinMode(MEASUREMENT_ERROR_LED, OUTPUT);

  // Start with all LEDs OFF.
  digitalWrite(SYSTEM_OK_LED, LOW);
  digitalWrite(MEASUREMENT_OK_LED, LOW);
  digitalWrite(MEASUREMENT_ERROR_LED, LOW);
}


// ==================================================
// UPDATE MAIN SYSTEM GREEN LED - D2
// ==================================================
//
// ON  -> INA219 + OLED + SD card are all available
// OFF -> at least one required device is unavailable
// ==================================================

void updateSystemStatusLED() {

  if (ina219Connected && oledConnected && sdConnected) {
    digitalWrite(SYSTEM_OK_LED, HIGH);
  }
  else {
    digitalWrite(SYSTEM_OK_LED, LOW);
  }
}


// ==================================================
// INDICATE MEASUREMENT RESULT
// ==================================================
//
// Successful measurement:
//   D3 turns ON for 40 ms and then OFF.
//
// Failed measurement:
//   D4 turns ON and stays ON until reset/power removal.
// ==================================================

void measurementLED(bool successful) {

  if (successful) {

    digitalWrite(MEASUREMENT_OK_LED, HIGH);
    delay(40);
    digitalWrite(MEASUREMENT_OK_LED, LOW);
  }
  else {

    // There is intentionally no command here to turn D4 back OFF.
    // Therefore, once an error occurs, the red LED remains ON.
    digitalWrite(MEASUREMENT_ERROR_LED, HIGH);
  }
}
