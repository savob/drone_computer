/* This file contains all the GPIO pin allocations, some pins have alternate names */

#ifndef DRONE_PINS_H
#define DRONE_PINS_H

#include <Arduino.h>

// LEDs, by number and colour
const uint8_t LED1 = PB3;
const uint8_t LED2 = PB4;
const uint8_t LED3 = PB5;

const uint8_t redLED = PB3;
const uint8_t greenLED1 = PB4;
const uint8_t greenLED2 = PB5;

// KILL SWITCH
const uint8_t killPin = PA1;

// Telemetry
const uint8_t teleIntPin = PA2;
const uint8_t teleCEPin = PA3;
const uint8_t teleCSpin = PA4;

// Battery Level
const uint8_t batteryPin = PA0;
#endif