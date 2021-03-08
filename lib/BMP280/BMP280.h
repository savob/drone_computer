/* TODO: 
    Add stuff to monitor altitude:
        - set baseline
        - altitude from baseline
        - estimation of altitude above sea level
    Investigate why temperature seems to constantly rise
*/

#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>
#include "Wire.h"

const u_int8_t RESET_REGISTER_BMP = 0xE0;
const u_int8_t ID_REGISTER_BMP = 0xD0;
const u_int8_t STATUS_REGISTER_BMP = 0xF3;
const u_int8_t CONTROL_MEAS_REGISTER_BMP = 0xF4;
const u_int8_t CONFIG_REGISTER_BMP = 0xF5;
const u_int8_t PRESSURE_REGISTER_BMP = 0xF7;
const u_int8_t TEMPERATURE_REGISTER_BMP = 0xFA;

// Constants
const u_int8_t RESET_VALUE = 0xB6; // When written to reset register, sensor resets
const u_int8_t BMP_ID = 0x58;      // ID stored in BMP280's

// Power modes
const u_int8_t POWER_MASK_BMP   = 3; // Mask used to pick out power settings
const u_int8_t POWER_SLEEP_BMP  = 0; // Put sensor to sleep
const u_int8_t POWER_FORCED_BMP = 1; // One measurement, then sleep
const u_int8_t POWER_NORMAL_BMP = 3; // Regular operation

// Standby times between measurements
const u_int8_t STANDBY_MASK_BMP = 7 << 5;
const u_int8_t STANDBY_00005_BMP = 0 << 5;
const u_int8_t STANDBY_00625_BMP = 1 << 5;
const u_int8_t STANDBY_01250_BMP = 2 << 5;
const u_int8_t STANDBY_02500_BMP = 3 << 5;
const u_int8_t STANDBY_05000_BMP = 4 << 5;
const u_int8_t STANDBY_10000_BMP = 5 << 5;
const u_int8_t STANDBY_20000_BMP = 6 << 5;
const u_int8_t STANDBY_40000_BMP = 7 << 5;

// Oversampling general
const u_int8_t OVERSAMPLE_00_BMP = 0;
const u_int8_t OVERSAMPLE_01_BMP = 1;
const u_int8_t OVERSAMPLE_02_BMP = 2;
const u_int8_t OVERSAMPLE_04_BMP = 3;
const u_int8_t OVERSAMPLE_08_BMP = 4;
const u_int8_t OVERSAMPLE_16_BMP = 5;

// Oversampling specific to each sensor value
const u_int8_t PRES_OVERSAMPLE_LS = 5;
const u_int8_t TEMP_OVERSAMPLE_LS = 3;
const u_int8_t PRES_OVERSAMPLE_MASK = 0xE0;
const u_int8_t TEMP_OVERSAMPLE_MASK = 0x1C;

// IIR Filter
const u_int8_t IIR_FILTER_LS = 2;
const u_int8_t IIR_FILTER_MASK = 0x1C;
const u_int8_t IIR_FILTER_00 = 0 << IIR_FILTER_LS;
const u_int8_t IIR_FILTER_02 = 1 << IIR_FILTER_LS;
const u_int8_t IIR_FILTER_04 = 2 << IIR_FILTER_LS;
const u_int8_t IIR_FILTER_08 = 3 << IIR_FILTER_LS;
const u_int8_t IIR_FILTER_16 = 4 << IIR_FILTER_LS;

// Trimmer values
// Stored T1 to T3, then P1 to P9.
const u_int8_t TRIM_START_REGISTER      = 0x88;
const u_int8_t TRIM_NUMBER_REGISTERS    = 24;

class BMP280I2C
{
private:
    // Bytes to determine sensor behavior
    u_int8_t configByte, controlMeasByte;
    u_int8_t I2CAddress = 0x77; // I2C address, defaults to 0x77

    // Trimmer values for calculations
    u_int16_t digT1, digP1;
    int16_t digT2, digT3, digP2, digP3, digP4, digP5, digP6, digP7, digP8, digP9;

    u_int32_t pressure;
    int32_t temperature;

    void sendBytes(u_int8_t, u_int8_t *, u_int8_t); // Used to to send a byte array of specific length
    void readBytes(u_int8_t, u_int8_t *, u_int8_t); // Used to to read a byte array of specific length

    bool ready(); // Returns true if sensor is ready to be read
public:
    BMP280I2C();
    BMP280I2C(u_int8_t);

    void reset(); // Resets the sensor

    bool checkIfPresent(); // Checks if there is actually a BMP280 at the address
    void setDefaults();    // Sets sensor to default values
    bool start();          // Used to start system, return true if good start
    bool start(u_int8_t);  // Used to start system, return true if good start

    void setI2CAddress(u_int8_t);
    u_int8_t readI2CAddress();

    void setPower(u_int8_t);
    void setTempOverSamp(u_int8_t);
    void setPresOverSamp(u_int8_t);
    void setStandbyTime(u_int8_t);
    void setFiltering(u_int8_t);

    void updateReadings();
    float readTempFloat();
    float readPresFloat();
    u_int32_t readPresLong();
    u_int16_t readTempInt();
};

#endif