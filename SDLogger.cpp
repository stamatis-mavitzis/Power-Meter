#include "SDLogger.h"

#include "Config.h"
#include "DeviceManager.h"
#include "Measurements.h"

// ==================================================
// CREATE DATA.csv IF NEEDED
// ==================================================
//
// If DATA.csv already exists, nothing is changed.
//
// If it does not exist, the file is created and the first line
// contains the CSV column names.
// ==================================================

void createDataFileIfNeeded() {

  if (!SD.exists(DATA_FILE_NAME)) {

    File dataFile = SD.open(DATA_FILE_NAME, FILE_WRITE);

    if (dataFile) {

      dataFile.println(
        F("Time_ms,Voltage_V,Current_mA,Power_mW,Energy_mWh")
      );

      // Always close the file after writing so the data is flushed
      // to the SD card and the file handle is released.
      dataFile.close();
    }
  }
}


// ==================================================
// APPEND ONE MEASUREMENT TO DATA.csv
// ==================================================
//
// Successful measurement example:
//   2500,8.921,145.200,1295.329,0.450123
//
// Failed measurement example:
//   2500,NA,NA,NA,0.450123
//
// Energy is still written during a failed measurement because it
// represents the accumulated energy from all earlier valid samples.
// ==================================================

bool logMeasurement(unsigned long currentMillis,
                    bool measurementSuccessful) {

  File dataFile = SD.open(DATA_FILE_NAME, FILE_WRITE);

  // Opening failed. In the original program this is treated as the
  // SD card becoming unavailable.
  if (!dataFile) {

    sdConnected = false;
    Serial.println(F("WARNING: SD card disconnected"));
    return false;
  }


  // ==================================================
  // TIME
  // ==================================================

  dataFile.print(currentMillis);
  dataFile.print(",");


  // ==================================================
  // MEASUREMENT VALUES
  // ==================================================

  if (measurementSuccessful) {

    dataFile.print(loadvoltage, 3);
    dataFile.print(",");

    dataFile.print(current_mA, 3);
    dataFile.print(",");

    dataFile.print(power_mW, 3);
    dataFile.print(",");

    dataFile.println(energy_mWh, 6);
  }
  else {

    // Keep the same number of CSV columns even when the INA219
    // measurement is unavailable.
    dataFile.print(F("NA,"));
    dataFile.print(F("NA,"));
    dataFile.print(F("NA,"));
    dataFile.println(energy_mWh, 6);
  }


  // Closing after every row makes the logger more resistant to data
  // loss if power is removed unexpectedly.
  dataFile.close();

  return true;
}
