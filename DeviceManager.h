#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include "SdFat.h"

// ==================================================
// DEVICE MANAGER
// ==================================================
//
// This file declares the hardware devices and the functions
// used to initialize/check them.
//
// DeviceManager is responsible for questions such as:
//   - Is the INA219 connected?
//   - Is the OLED connected?
//   - Is the SD card available?
//   - Can the INA219 reconnect after being unplugged?
//
// Measurement calculations are NOT done here.
// OLED drawing is NOT done here.
// CSV logging is NOT done here.
// ==================================================


// ==================================================
// HARDWARE OBJECTS
// ==================================================
//
// These objects are created once in DeviceManager.cpp.
// The word "extern" tells the other files that the objects
// exist somewhere else in the project.
// ==================================================

extern SSD1306AsciiWire display;
extern Adafruit_INA219 ina219;
extern SdFat SD;


// ==================================================
// DEVICE STATUS FLAGS
// ==================================================
//
// true  = device is currently considered available
// false = device is missing/not working
// ==================================================

extern bool ina219Connected;
extern bool oledConnected;
extern bool sdConnected;

// Actual OLED I2C address detected at startup (0x3C or 0x3D).
extern uint8_t oledAddress;


// ==================================================
// DEVICE-MANAGER FUNCTIONS
// ==================================================

// Checks whether an I2C device answers at the given address.
bool i2cDevicePresent(uint8_t address);

// Starts the Arduino hardware I2C bus and enables timeout protection.
void initializeI2C();

// Checks and initializes INA219, OLED and SD card during setup().
void initializeDevices();

// Checks the INA219 before each measurement.
// If it was disconnected and comes back, this function tries to reconnect it.
// Returns true when the INA219 is ready to be used.
bool ensureINA219Connected();

// Checks whether an already-initialized OLED still answers on I2C.
// Returns true when the display can safely be updated.
bool ensureOLEDConnected();

// Checks the SD card during every measurement cycle.
// If the SD was previously missing/disconnected, this function attempts
// to initialize it again. Returns true when the SD card is ready.
bool ensureSDCardConnected();

#endif
