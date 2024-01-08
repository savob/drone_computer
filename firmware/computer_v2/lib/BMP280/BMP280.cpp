#include "BMP280.h"

BMP280I2C::BMP280I2C()
{
}
BMP280I2C::BMP280I2C(u_int8_t address)
{
    I2CAddress = address;
    BMP280I2C();
}
u_int8_t BMP280I2C::readI2CAddress()
{
    return I2CAddress;
}
void BMP280I2C::setI2CAddress(u_int8_t add)
{
    I2CAddress = add;
}
bool BMP280I2C::start()
{
    Wire.begin();

    // Check if it is present at the right address
    if (checkIfPresent() == true)
    {
        setDefaults();

        // Read in trimmer values used to calculate values
        u_int8_t trimBuffer[TRIM_NUMBER_REGISTERS];
        readBytes(TRIM_START_REGISTER, trimBuffer, TRIM_NUMBER_REGISTERS);

        // Store them
        digT1 = (u_int16_t(trimBuffer[1]) << 8) + trimBuffer[0];
        digT2 = (int16_t(trimBuffer[3]) << 8) + trimBuffer[2];
        digT3 = (int16_t(trimBuffer[5]) << 8) + trimBuffer[4];
        digP1 = (u_int16_t(trimBuffer[7]) << 8) + trimBuffer[6];
        digP2 = (int16_t(trimBuffer[9]) << 8) + trimBuffer[8];
        digP3 = (int16_t(trimBuffer[11]) << 8) + trimBuffer[10];
        digP4 = (int16_t(trimBuffer[13]) << 8) + trimBuffer[12];
        digP5 = (int16_t(trimBuffer[15]) << 8) + trimBuffer[14];
        digP6 = (int16_t(trimBuffer[17]) << 8) + trimBuffer[16];
        digP7 = (int16_t(trimBuffer[19]) << 8) + trimBuffer[18];
        digP8 = (int16_t(trimBuffer[21]) << 8) + trimBuffer[20];
        digP9 = (int16_t(trimBuffer[23]) << 8) + trimBuffer[22];
    }
    else
    {
        return false; // No sensor seen or an incorrect sensor detected, alert system
    }

    return true;
}
bool BMP280I2C::start(u_int8_t add)
{
    // Start but with a potentially new address specified
    setI2CAddress(add);
    return start();
}

void BMP280I2C::sendBytes(u_int8_t startRegister, u_int8_t *dataToSend, u_int8_t lengthOfData) // Writes a specified amount of bytes to the sensor
{
    Wire.beginTransmission(I2CAddress);

    // Send the byte array to registers
    // BMP280 requires the register address and then the value for each individual register
    for (u_int8_t i = 0; i < lengthOfData; i++) {
        Wire.write(startRegister + i);
        Wire.write(dataToSend[i]);
    }

    Wire.endTransmission();
}

void BMP280I2C::readBytes(u_int8_t startRegister, u_int8_t *bufferToStore, u_int8_t lengthOfData) // Reads a specified amount of bytes from the BMP
{
    // Send register we want to start reading from
    Wire.beginTransmission(I2CAddress);
    Wire.write(startRegister);
    Wire.endTransmission();

    Wire.requestFrom(I2CAddress, lengthOfData);
    // Read it in byte by byte
    for (u_int8_t i = 0; i < lengthOfData; i++)
        bufferToStore[i] = Wire.read();
}

void BMP280I2C::reset() // Resets the BMP280
{
    u_int8_t temp = RESET_VALUE;
    sendBytes(RESET_REGISTER_BMP, &temp, 1);
}

bool BMP280I2C::ready() // Returns true if not mid-measurement of copy
{
    u_int8_t temp = 0;
    readBytes(STATUS_REGISTER_BMP, &temp, 1);

    if (temp > 0)
        return (false); // Return false if any status bit is true
    else
        return (true); // Return true if ok to read
}
bool BMP280I2C::checkIfPresent() // Returns true if BMP280 present at address
{
    u_int8_t temp = 0;
    readBytes(ID_REGISTER_BMP, &temp, 1);

    if (temp == BMP_ID)
        return true;
    else
        return false;
}
void BMP280I2C::setDefaults() // Returns true if BMP280 present at address
{
    // Reset existing settings
    configByte = 0;
    controlMeasByte = 0;

    setStandbyTime(STANDBY_00005_BMP);
    setFiltering(IIR_FILTER_00);
    setPower(POWER_NORMAL_BMP);
    setTempOverSamp(OVERSAMPLE_02_BMP);
    setPresOverSamp(OVERSAMPLE_16_BMP);
}
void BMP280I2C::setPower(u_int8_t setting)
{
    controlMeasByte &= ~POWER_MASK_BMP; // Clear power bits
    controlMeasByte |= setting;         // Set needed bits

    sendBytes(CONTROL_MEAS_REGISTER_BMP, &controlMeasByte, 1);
}
void BMP280I2C::setTempOverSamp(u_int8_t setting)
{
    controlMeasByte &= ~TEMP_OVERSAMPLE_MASK; // Clear old bits
    controlMeasByte |= setting << TEMP_OVERSAMPLE_LS;

    sendBytes(CONTROL_MEAS_REGISTER_BMP, &controlMeasByte, 1);
}
void BMP280I2C::setPresOverSamp(u_int8_t setting)
{
    controlMeasByte &= ~PRES_OVERSAMPLE_MASK; // Clear old bits
    controlMeasByte |= setting << PRES_OVERSAMPLE_LS;

    sendBytes(CONTROL_MEAS_REGISTER_BMP, &controlMeasByte, 1);
}
void BMP280I2C::setStandbyTime(u_int8_t setting)
{
    configByte &= ~STANDBY_MASK_BMP; // Clear old bits
    configByte |= setting;

    // Load onto sensor
    sendBytes(CONFIG_REGISTER_BMP, &configByte, 1);
}
void BMP280I2C::setFiltering(u_int8_t setting)
{
    configByte &= ~IIR_FILTER_MASK; // Clear old bits
    configByte |= setting << IIR_FILTER_LS;

    // Load onto sensor
    sendBytes(CONFIG_REGISTER_BMP, &configByte, 1);
}

void BMP280I2C::updateReadings()
{
    // Read in ADC values for both values (temperature is needed for pressure)
    u_int8_t ADCBuffer[6];
    readBytes(PRESSURE_REGISTER_BMP, ADCBuffer, 6);

    int32_t pressureADC = (int32_t(ADCBuffer[0]) << 12) + (int32_t(ADCBuffer[1]) << 4) + (int32_t(ADCBuffer[2]) >> 4);
    int32_t temperatureADC = (int32_t(ADCBuffer[3]) << 12) + (int32_t(ADCBuffer[4]) << 4) + (int32_t(ADCBuffer[5]) >> 4);

    /* Sample code taken form datasheet for BMP280, modified only slightly */
    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
    int32_t var1, var2, tempFine;

    var1 = ((((temperatureADC >> 3) - ((int32_t)digT1 << 1))) * ((int32_t)digT2)) >> 11;
    var2 = (((((temperatureADC >> 4) - ((int32_t)digT1)) * ((temperatureADC >> 4) - ((int32_t)digT1))) >> 12) * ((int32_t)digT3)) >> 14;
    tempFine = var1 + var2;
    temperature = (tempFine * 5 + 128) >> 8; // Temperature * 100

    // Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
    var1 = (((int32_t)tempFine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)digP6);
    var2 = var2 + ((var1 * ((int32_t)digP5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)digP4) << 16);
    var1 = (((digP3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)digP2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)digP1)) >> 15);
    if (var1 == 0)
        return; // avoid exception caused by division by zero

    pressure = (((u_int32_t)(((int32_t)1048576) - pressureADC) - (var2 >> 12))) * 3125;
    if (pressure < 0x80000000)
        pressure = (pressure << 1) / ((u_int32_t)var1);

    else
        pressure = (pressure / (u_int32_t)var1) * 2;

    var1 = (((int32_t)digP9) * ((int32_t)(((pressure >> 3) * (pressure >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(pressure >> 2)) * ((int32_t)digP8)) >> 13;
    pressure = (u_int32_t)((int32_t)pressure + ((var1 + var2 + digP7) >> 4));
}
float BMP280I2C::readTempFloat()
{
    return (float(temperature) / 100.0);
}
float BMP280I2C::readPresFloat()
{
    return (float(pressure) / 100.0);
}
u_int32_t BMP280I2C::readPresLong()
{
    return pressure;
}
u_int16_t BMP280I2C::readTempInt()
{
    return temperature;
}