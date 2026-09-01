#include "StatusLED.h"

#include <Arduino.h>

#include "Config.h"
#include "DeviceManager.h"

// ==================================================
// LATCHED SYSTEM-STATUS STATE
// ==================================================
//
// false -> no INA219/SD failure has occurred since startup
// true  -> INA219 or SD failed at least once since startup
//
// This variable is reset only when the Arduino resets or powers up.
// ==================================================

static bool systemOkLatchedOff = false;


// ==================================================
// PRIVATE ERROR-LED HELPER
// ==================================================
//
// D4 is intentionally never turned OFF here. Once it is switched ON,
// it stays ON until reset or power removal.
// ==================================================

static void latchErrorLED() {
  digitalWrite(MEASUREMENT_ERROR_LED, HIGH);
}


// ==================================================
// CONFIGURE LED PINS
// ==================================================

void setupStatusLEDs() {

  pinMode(SYSTEM_OK_LED, OUTPUT);
  pinMode(MEASUREMENT_OK_LED, OUTPUT);
  pinMode(MEASUREMENT_ERROR_LED, OUTPUT);

  // A new boot/reset clears all latched LED states.
  systemOkLatchedOff = false;

  digitalWrite(SYSTEM_OK_LED, LOW);
  digitalWrite(MEASUREMENT_OK_LED, LOW);
  digitalWrite(MEASUREMENT_ERROR_LED, LOW);
}


// ==================================================
// UPDATE MAIN SYSTEM GREEN LED - D2
// ==================================================
//
// D2 depends ONLY on the INA219 and SD card.
//
// At startup:
//   ON  -> INA219 and SD card are both available
//   OFF -> INA219 or SD card is unavailable
//
// During operation:
//   If INA219 or SD becomes unavailable even once, D2 turns OFF and
//   remains OFF until the Arduino is reset or power-cycled.
//
// OLED status does NOT affect D2.
//
// Any INA219/SD availability failure also latches the red D4 LED ON.
// ==================================================

void updateSystemStatusLED() {

  // Any failure of either critical device permanently latches the
  // System OK LED OFF for the rest of this boot session.
  if (!ina219Connected || !sdConnected) {
    systemOkLatchedOff = true;
    latchErrorLED();
  }

  if (!systemOkLatchedOff && ina219Connected && sdConnected) {
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
// Failed / invalid measurement:
//   D4 turns ON and remains ON until reset/power removal.
// ==================================================

void measurementLED(bool successful) {

  if (successful) {

    digitalWrite(MEASUREMENT_OK_LED, HIGH);
    delay(40);
    digitalWrite(MEASUREMENT_OK_LED, LOW);
  }
  else {

    latchErrorLED();
  }
}
