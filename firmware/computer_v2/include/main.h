#ifndef MAIN_DRONE
#define MAIN_DRONE

#include <Arduino.h>
#include "SPI.h"
#include "pinAllocations.h" // Pin allocations stored here
#include "BMP280.h"
#include "ICM20600.h"
#include "telemetry.h"

// Constants
const float batteryScaling = 7.0;   // Multiplied by analog reading to deduce battery voltage

const bool LEDOn = HIGH;            // What state to set LEDs to turn on/off
const bool LEDOff = !LEDOn;

const u_int32_t USBbaud = 115200;       // Baudrate used in PC communication (doesn't really matter with native USB)

// I2C Address
const u_int8_t BMPAddress = 0x77;   // Address of pressure sensor on I2C
const u_int8_t IMUAddress = 0x69;   // Address of IMU on I2C

//===============================
// Functions
void setupMCUPins(); // Used to setup pins
#endif