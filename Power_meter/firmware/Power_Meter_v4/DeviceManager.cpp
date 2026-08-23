#include "DeviceManager.h"

#include <Wire.h>

#include "Config.h"
#include "Display.h"
#include "SDLogger.h"

// ==================================================
// DEVICE OBJECT DEFINITIONS
// ==================================================
//
// The corresponding declarations are in DeviceManager.h.
// There must be only ONE actual definition of each object.
// ==================================================

Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219(INA219_ADDRESS);
SdFat SD;


// ==================================================
// DEVICE STATUS DEFINITIONS
// ==================================================

bool ina219Connected = false;
bool oledConnected = false;
bool sdConnected = false;

// The OLED address is detected during startup.
uint8_t oledAddress = OLED_ADDRESS_PRIMARY;


// ==================================================
// PRIVATE STARTUP FUNCTIONS
// ==================================================
//
// "static" means these helper functions are used only inside
// DeviceManager.cpp. Other project files do not need to see them.
// ==================================================

static void initializeINA219();
static void initializeOLED();
static void initializeSDCard();


// ==================================================
// CHECK IF AN I2C DEVICE EXISTS
// ==================================================
//
// Wire.beginTransmission() + Wire.endTransmission() sends a small
// I2C transaction to the requested address.
//
// error == 0 means the device answered.
// ==================================================

bool i2cDevicePresent(uint8_t address) {

  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();

#if defined(WIRE_HAS_TIMEOUT)

  // If the I2C bus timed out, clear the timeout flag so later
  // I2C operations can continue normally.
  if (Wire.getWireTimeoutFlag()) {
    Wire.clearWireTimeoutFlag();
    return false;
  }

#endif

  return (error == 0);
}


// ==================================================
// START HARDWARE I2C
// ==================================================
//
// On the Arduino Pro Mini / ATmega328P:
//   SDA = A4
//   SCL = A5
// ==================================================

void initializeI2C() {

  Wire.begin();

#if defined(WIRE_HAS_TIMEOUT)

  // Prevent a broken/disconnected I2C device from freezing the
  // entire Arduino forever.
  Wire.setWireTimeout(5000, true);

#endif
}


// ==================================================
// INITIALIZE ALL DEVICES
// ==================================================
//
// Called once from setup().
// Each individual device prints its startup condition to Serial.
// ==================================================

void initializeDevices() {

  initializeINA219();
  initializeOLED();
  initializeSDCard();
}


// ==================================================
// INITIALIZE INA219
// ==================================================

static void initializeINA219() {

  // First check whether something answers at the configured INA219 address.
  if (i2cDevicePresent(INA219_ADDRESS)) {

    // Then ask the Adafruit INA219 library to initialize the sensor.
    if (ina219.begin()) {
      ina219Connected = true;
      Serial.println(F("INA219: CONNECTED"));
    }
    else {
      ina219Connected = false;
      Serial.println(F("INA219: NOT WORKING"));
    }
  }
  else {
    ina219Connected = false;
    Serial.println(F("INA219: NOT CONNECTED"));
  }
}


// ==================================================
// INITIALIZE OLED
// ==================================================

static void initializeOLED() {

  // The HR0088 0.96" OLED is a 128x64 I2C display.
  // Most boards use 0x3C, while some variants are configured as 0x3D.
  // Check both so the sketch does not depend on guessing the address.
  if (i2cDevicePresent(OLED_ADDRESS_PRIMARY)) {
    oledAddress = OLED_ADDRESS_PRIMARY;
  }
  else if (i2cDevicePresent(OLED_ADDRESS_SECONDARY)) {
    oledAddress = OLED_ADDRESS_SECONDARY;
  }
  else {
    oledConnected = false;
    Serial.println(F("OLED: NOT CONNECTED"));
    return;
  }

  // The display object was constructed explicitly as 128x64 hardware I2C.
  // SSD1306_SWITCHCAPVCC is the normal setting for this type of module.
  // The final argument is false because initializeI2C() already called Wire.begin().
  if (display.begin(SSD1306_SWITCHCAPVCC, oledAddress, true, false)) {
    oledConnected = true;

    Serial.print(F("OLED: CONNECTED at 0x"));
    if (oledAddress < 0x10) {
      Serial.print('0');
    }
    Serial.println(oledAddress, HEX);

    // The actual drawing code is kept in Display.cpp.
    showStartupScreen();
  }
  else {
    oledConnected = false;
    Serial.println(F("OLED: NOT WORKING"));
  }
}


// ==================================================
// INITIALIZE SD CARD
// ==================================================
//
// Hardware SPI pins are automatically selected by the Arduino:
//   MOSI = D11
//   MISO = D12
//   SCK  = D13
//   CS   = D10
// ==================================================

static void initializeSDCard() {

  if (SD.begin(SD_CHIP_SELECT)) {

    sdConnected = true;
    Serial.println(F("SD CARD: CONNECTED"));

    // Create DATA.csv and its header only when the file does not exist.
    createDataFileIfNeeded();
  }
  else {
    sdConnected = false;
    Serial.println(F("SD CARD: NOT CONNECTED"));
  }
}


// ==================================================
// CHECK / RECONNECT INA219 DURING OPERATION
// ==================================================
//
// Called before every measurement.
//
// This preserves the behavior of your original code:
// if the INA219 disappears, the code marks it disconnected.
// If it later responds again, ina219.begin() is attempted again.
// ==================================================

bool ensureINA219Connected() {

  if (i2cDevicePresent(INA219_ADDRESS)) {

    // If the device answers but was previously marked disconnected,
    // try to initialize it again.
    if (!ina219Connected) {

      if (ina219.begin()) {
        ina219Connected = true;
        Serial.println(F("INA219 reconnected"));
      }
    }

    return ina219Connected;
  }

  // The INA219 no longer answers at its I2C address.
  if (ina219Connected) {
    Serial.println(F("WARNING: INA219 disconnected"));
  }

  ina219Connected = false;
  return false;
}


// ==================================================
// CHECK OLED DURING OPERATION
// ==================================================
//
// The original program only checks an OLED that successfully
// initialized at startup. It does not try to reconnect an OLED
// that was missing when the Arduino powered on.
// ==================================================

bool ensureOLEDConnected() {

  // If startup already marked the OLED unavailable, do nothing.
  if (!oledConnected) {
    return false;
  }

  // OLED still answers -> it is safe for Display.cpp to use it.
  if (i2cDevicePresent(oledAddress)) {
    return true;
  }

  // OLED was available but stopped answering.
  oledConnected = false;
  Serial.println(F("WARNING: OLED disconnected"));

  return false;
}
