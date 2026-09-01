#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <Arduino.h>

// ==================================================
// MEASUREMENTS
// ==================================================
//
// This file contains the values measured/calculated by the
// power meter and declares the functions used to update them.
//
// It is responsible for:
//   - reading INA219 values
//   - calculating load voltage
//   - calculating power
//   - accumulating energy
//
// It is NOT responsible for displaying or saving the values.
// ==================================================


// ==================================================
// MEASUREMENT VARIABLES
// ==================================================
//
// "extern" means the actual variables are created once in
// Measurements.cpp and can be used by other files.
// ==================================================

extern float shuntvoltage;
extern float busvoltage;
extern float current_mA;
extern float loadvoltage;
extern float power_mW;
extern float energy_mWh;


// ==================================================
// MEASUREMENT FUNCTIONS
// ==================================================

// Reads INA219 and calculates voltage/current/power.
// Returns true when all values are finite and valid.
bool readINA219();

// Clears the instantaneous readings when a measurement cannot be taken.
// Energy is deliberately NOT reset because it is the accumulated total.
void resetMeasurementValues();

// Adds the energy produced during elapsedMillis to energy_mWh.
void updateEnergy(unsigned long elapsedMillis);

#endif
