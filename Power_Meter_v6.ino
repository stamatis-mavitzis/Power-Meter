#include "Config.h"
#include "DeviceManager.h"
#include "Measurements.h"
#include "Display.h"
#include "SDLogger.h"
#include "StatusLED.h"

// ==================================================
// POWER METER - MAIN PROGRAM
// ==================================================
//
// This is intentionally the smallest/high-level file in the project.
//
// Its job is to control the ORDER of operations:
//
//   1. Initialize the system
//   2. Wait for the next 100 ms measurement point
//   3. Check the INA219
//   4. Read voltage/current/power
//   5. Calculate energy
//   6. Update LEDs
//   7. Print to Serial
//   8. Update OLED
//   9. Save to SD card
//
// The detailed implementation of each job is kept in the other files.
// ==================================================


// Stores the time at which the previous measurement was taken.
unsigned long previousMillis = 0;


// ==================================================
// PRINT ONE MEASUREMENT TO SERIAL
// ==================================================
//
// This helper stays in PowerMeter.ino because Serial Monitor output
// is part of the main program flow rather than a separate hardware
// subsystem in this project.
// ==================================================

void printSerialMeasurement(unsigned long currentMillis,
                            bool measurementSuccessful) {

  Serial.print(currentMillis);
  Serial.print(",");

  if (measurementSuccessful) {

    Serial.print(loadvoltage, 3);
    Serial.print(",");

    Serial.print(current_mA, 3);
    Serial.print(",");

    Serial.print(power_mW, 3);
    Serial.print(",");

    Serial.println(energy_mWh, 6);
  }
  else {

    // Use NA instead of false measurement values.
    Serial.print(F("NA,"));
    Serial.print(F("NA,"));
    Serial.print(F("NA,"));
    Serial.println(energy_mWh, 6);
  }
}


// ==================================================
// SETUP
// ==================================================
//
// Runs once when the Arduino powers on or resets.
// ==================================================

void setup() {

  // Configure D2, D3 and D4 and turn all LEDs OFF.
  setupStatusLEDs();


  // ==================================================
  // SERIAL MONITOR
  // ==================================================

  Serial.begin(SERIAL_BAUD);

  Serial.println();
  Serial.println(F("Power Meter Starting..."));
  Serial.println(F("-----------------------"));


  // ==================================================
  // I2C
  // ==================================================
  //
  // Starts hardware I2C on:
  //   SDA = A4
  //   SCL = A5
  // ==================================================

  initializeI2C();


  // ==================================================
  // INA219 + OLED + SD CARD
  // ==================================================

  initializeDevices();


  // Evaluate the two critical devices immediately after startup.
  // D2 is ON only if INA219 + SD are both available.
  // If either one is missing, D2 latches OFF and D4 latches ON.
  updateSystemStatusLED();


  // ==================================================
  // SERIAL CSV HEADER
  // ==================================================

  Serial.println(F("-----------------------"));
  Serial.println(
    F("Time_ms,Voltage_V,Current_mA,Power_mW,Energy_mWh")
  );
}


// ==================================================
// MAIN LOOP
// ==================================================
//
// Runs continuously while the Arduino is powered.
// ==================================================

void loop() {

  unsigned long currentMillis = millis();


  // ==================================================
  // WAIT UNTIL THE NEXT MEASUREMENT INTERVAL
  // ==================================================

  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {

    // Actual time since the previous measurement.
    // This is used for energy integration.
    unsigned long elapsedMillis = currentMillis - previousMillis;

    // Save the timestamp of this measurement.
    previousMillis = currentMillis;

    // Assume failure until a valid INA219 reading proves otherwise.
    bool measurementSuccessful = false;


    // ==================================================
    // INA219 CONNECTION CHECK
    // ==================================================

    if (ensureINA219Connected()) {

      // ==================================================
      // TAKE MEASUREMENT
      // ==================================================

      measurementSuccessful = readINA219();


      // ==================================================
      // ACCUMULATE ENERGY
      // ==================================================

      if (measurementSuccessful) {
        updateEnergy(elapsedMillis);
      }
    }
    else {

      // INA219 is unavailable, so do not leave old voltage/current/
      // power values in memory as if they were the latest sample.
      resetMeasurementValues();
    }


    // ==================================================
    // MEASUREMENT LEDs
    // ==================================================
    //
    // Success -> D3 flashes.
    // Failure -> D4 turns ON permanently.
    // ==================================================

    measurementLED(measurementSuccessful);


    // ==================================================
    // SERIAL OUTPUT
    // ==================================================

    printSerialMeasurement(currentMillis, measurementSuccessful);


    // ==================================================
    // OLED OUTPUT
    // ==================================================

    if (ensureOLEDConnected()) {
      displayData();
    }


    // ==================================================
    // SD CARD CHECK + OUTPUT
    // ==================================================
    //
    // The SD card is checked during every measurement cycle.
    //
    // - If it is currently connected, logMeasurement() verifies it by
    //   opening DATA.csv and writing the current row.
    // - If it was missing/disconnected, ensureSDCardConnected() attempts
    //   to initialize it again on this cycle.
    //
    // Any SD failure is latched by updateSystemStatusLED():
    //   D2 -> OFF until reset/power cycle
    //   D4 -> ON  until reset/power cycle
    // ==================================================

    if (ensureSDCardConnected()) {
      logMeasurement(currentMillis, measurementSuccessful);
    }


    // ==================================================
    // UPDATE LATCHED SYSTEM / ERROR LEDs
    // ==================================================
    //
    // This runs after the INA219 check and after the SD-card check/write,
    // so a disconnection discovered during this measurement cycle is
    // reflected immediately in the LEDs.
    // ==================================================

    updateSystemStatusLED();
  }
}
