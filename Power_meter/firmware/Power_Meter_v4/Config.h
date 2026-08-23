#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==================================================
// CONFIGURATION FILE
// ==================================================
//
// This file contains values that describe the hardware
// and the general settings of the power meter.
//
// If you change a pin, I2C address, measurement interval,
// or Serial baud rate, this is the first file to edit.
//
// Keeping these values here prevents "magic numbers" from
// being scattered throughout the rest of the project.
// ==================================================


// ==================================================
// PINOUT
// ==================================================
//
// SD CARD - Hardware SPI
// -----------------------
// CS   -> D10
// MOSI -> D11
// MISO -> D12
// SCK  -> D13
//
// INA219 - Hardware I2C
// -----------------------
// SDA -> A4
// SCL -> A5
//
// OLED - Hardware I2C
// -----------------------
// SDA -> A4
// SCL -> A5
//
// LEDs
// -----------------------
// System OK               -> D2
// Successful measurement  -> D3
// Measurement error        -> D4
// ==================================================


// ==================================================
// I2C DEVICE SETTINGS
// ==================================================

// Exact OLED used in this project:
//   OLED Module 0.96" 128x64 - I2C White (HR0088)
//
// The display has 128 x 64 pixels and uses the two-wire I2C bus.
// The module has no separate RESET wire, so -1 tells the Adafruit
// library not to use an Arduino reset pin.
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64
#define OLED_RESET -1

// Most 0.96" I2C OLED modules use 0x3C. Some variants use 0x3D.
// The code automatically checks both addresses during startup.
#define OLED_ADDRESS_PRIMARY 0x3C
#define OLED_ADDRESS_SECONDARY 0x3D

// I2C address detected by the I2C scanner for this INA219 module.
#define INA219_ADDRESS 0x45


// ==================================================
// LED PINS
// ==================================================

// Green LED: ON only when INA219 + OLED + SD card are all available.
#define SYSTEM_OK_LED 2

// Green LED: flashes after a successful measurement.
#define MEASUREMENT_OK_LED 3

// Red LED: turns ON permanently after a failed measurement.
#define MEASUREMENT_ERROR_LED 4


// ==================================================
// SD CARD SETTINGS
// ==================================================

// Hardware SPI automatically uses D11, D12 and D13.
// Only the Chip Select pin has to be specified by the program.
const uint8_t SD_CHIP_SELECT = 10;

// File used to store all power-meter measurements.
#define DATA_FILE_NAME "DATA.csv"


// ==================================================
// TIMING SETTINGS
// ==================================================

// Take one measurement every 100 milliseconds.
const unsigned long MEASUREMENT_INTERVAL_MS = 100;

// Serial Monitor speed.
const unsigned long SERIAL_BAUD = 9600;

#endif
