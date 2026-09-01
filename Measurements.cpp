#include "Measurements.h"

#include <math.h>

#include "DeviceManager.h"

// ==================================================
// MEASUREMENT VARIABLE DEFINITIONS
// ==================================================

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
float energy_mWh = 0;


// ==================================================
// RESET CURRENT MEASUREMENT VALUES
// ==================================================
//
// Used when the INA219 is disconnected or returns invalid data.
//
// energy_mWh is NOT cleared here because it stores the energy
// accumulated since the Arduino started.
// ==================================================

void resetMeasurementValues() {

  shuntvoltage = 0;
  busvoltage = 0;
  current_mA = 0;
  loadvoltage = 0;
  power_mW = 0;
}


// ==================================================
// READ INA219
// ==================================================
//
// Returns:
//   true  -> valid measurement
//   false -> failed / invalid measurement
// ==================================================

bool readINA219() {

  // Read the raw measurement values from the INA219 library.
  float newShuntVoltage = ina219.getShuntVoltage_mV();
  float newBusVoltage = ina219.getBusVoltage_V();
  float newCurrent = ina219.getCurrent_mA();


  // ==================================================
  // CHECK RAW VALUES
  // ==================================================
  //
  // isfinite() rejects NaN and infinity. These values can appear
  // when communication with the device fails or data becomes invalid.
  // ==================================================

  if (!isfinite(newShuntVoltage) ||
      !isfinite(newBusVoltage) ||
      !isfinite(newCurrent)) {

    resetMeasurementValues();
    return false;
  }


  // ==================================================
  // SAVE RAW VALUES
  // ==================================================

  shuntvoltage = newShuntVoltage;
  busvoltage = newBusVoltage;
  current_mA = newCurrent;


  // ==================================================
  // CALCULATE LOAD VOLTAGE
  // ==================================================
  //
  // INA219 reports:
  //   busvoltage   in volts
  //   shuntvoltage in millivolts
  //
  // Divide shunt voltage by 1000 to convert mV -> V.
  // ==================================================

  loadvoltage = busvoltage + (shuntvoltage / 1000.0);


  // ==================================================
  // CALCULATE POWER
  // ==================================================
  //
  // V x mA = mW
  // ==================================================

  power_mW = loadvoltage * current_mA;


  // ==================================================
  // FINAL VALIDITY CHECK
  // ==================================================

  if (!isfinite(loadvoltage) || !isfinite(power_mW)) {

    // These are calculated values, so clear the whole instantaneous
    // measurement if they become invalid.
    resetMeasurementValues();
    return false;
  }

  return true;
}


// ==================================================
// ACCUMULATE ENERGY
// ==================================================
//
// Energy = Power x Time
//
// power_mW is in milliwatts.
// elapsedMillis is converted from milliseconds to hours.
//
// mW x h = mWh
// ==================================================

void updateEnergy(unsigned long elapsedMillis) {

  energy_mWh +=
    power_mW * ((float)elapsedMillis / 3600000.0);
}
