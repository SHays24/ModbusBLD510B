#include "BLD510B.h"
#include "ModBusBuilder.h"


BLD510B::BLD510B(
    uint8_t server,
    bool motorDir
)
{
    this->addr = server;
    this->motorDirection = motorDir;
}


bool BLD510B::setSpeed(
    uint16_t RPM
)
{
    uint8_t frame[8];

    uint8_t data1 = RPM & 0xFF;
    uint8_t data2 = (RPM >> 8) & 0xFF;

    size_t len = mbBuilder.buildWriteSingleRegister(
        this->addr,
        0x8005,
        data1,
        data2,
        frame
    );

    Serial1.write(frame, len);

    return true;
}


bool BLD510B::setControl(
    uint8_t control,
    uint8_t polePairs
)
{
    uint8_t frame[8];

    size_t len = mbBuilder.buildWriteSingleRegister(
        this->addr,
        0x8000,
        control,
        polePairs,
        frame
    );

    Serial1.write(frame, len);

    return true;
}


uint16_t BLD510B::getSpeed()
{
    uint8_t frame[8];

    size_t len = mbBuilder.buildReadRegisters(
        this->addr,
        0x8018,
        1,
        frame
    );

    // Clear old received data
    while (Serial1.available())
    {
        Serial1.read();
    }

    // Send request
    Serial1.write(frame, len);
    Serial1.flush();

    // Expected Modbus response:
    //
    // Address
    // Function
    // Byte count
    // Data high
    // Data low
    // CRC low
    // CRC high

    uint8_t response[7];

    unsigned long startTime = millis();

    while (Serial1.available() < 7)
    {
        if (millis() - startTime > 100)
        {
            return 0;
        }
    }

    size_t received = Serial1.readBytes(
        response,
        7
    );

    if (received != 7)
    {
        return 0;
    }

    // Check address
    if (response[0] != this->addr)
    {
        return 0;
    }

    // Check function
    if (response[1] != 0x03)
    {
        return 0;
    }

    // Check byte count
    if (response[2] != 2)
    {
        return 0;
    }

    // Convert two bytes to 16-bit register
    uint16_t speed =
        ((uint16_t)response[3] << 8) |
        response[4];

    return speed;
}
