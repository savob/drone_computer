#include "ICM20600.h"

class ICM20600I2C
{
private:
    u_int8_t I2CAddress = 0x69;

    void sendBytes(u_int8_t, u_int8_t *, u_int8_t); // Used to to send a byte array of specific length
    void readBytes(u_int8_t, u_int8_t *, u_int8_t); // Used to to read a byte array of specific length
public:
    bool start();
    bool start(u_int8_t);
    void setDefaults();

    void setI2CAddress(u_int8_t);
    bool checkIfPresent();  // Checks if device at address is actually ICM20600
};

void ICM20600I2C::setDefaults() {

}
bool ICM20600I2C::start()
{
    Wire.begin();

    // Check if it is present at the right address
    if (checkIfPresent() == true)
    {
        setDefaults();

        // Read in trimmer values used to calculate values
    }
    else
    {
        return false; // No sensor seen or an incorrect sensor detected, alert system
    }

    return true;
}
bool ICM20600I2C::start(u_int8_t add)
{
    // Start but with a potentially new address specified
    setI2CAddress(add);
    return start();
}
void ICM20600I2C::setI2CAddress(u_int8_t add)
{
    I2CAddress = add;
}

bool ICM20600I2C::checkIfPresent()
{
    u_int8_t temp = 0;
    readBytes(ID_REGISTER_ICM20600, &temp, 1);

    if (temp == ID_ICM20600)
        return true;
    else
        return false;
}

void ICM20600I2C::sendBytes(u_int8_t startRegister, u_int8_t *dataToSend, u_int8_t lengthOfData) // Writes a specified amount of bytes to the sensor
{
    Wire.beginTransmission(I2CAddress);

    // Send the byte array to registers
    // BMP280 requires the register address and then the value for each individual register
    for (u_int8_t i = 0; i < lengthOfData; i++)
    {
        Wire.write(startRegister + i);
        Wire.write(dataToSend[i]);
    }

    Wire.endTransmission();
}

void ICM20600I2C::readBytes(u_int8_t startRegister, u_int8_t *bufferToStore, u_int8_t lengthOfData) // Reads a specified amount of bytes from the BMP
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