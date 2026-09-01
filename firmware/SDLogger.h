#ifndef SD_LOGGER_H
#define SD_LOGGER_H

#include <Arduino.h>

// ==================================================
// SD CARD LOGGER
// ==================================================
//
// This file declares functions for creating DATA.csv and
// adding one measurement row to it.
//
// SD hardware initialization belongs to DeviceManager.
// Measurement calculations belong to Measurements.
// ==================================================

// Creates DATA.csv and its CSV header only if the file does not exist.
void createDataFileIfNeeded();

// Appends one measurement row to DATA.csv.
// Returns true when the row was written successfully.
// Returns false if the file could not be opened.
bool logMeasurement(unsigned long currentMillis,
                    bool measurementSuccessful);

#endif
