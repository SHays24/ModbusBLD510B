#include "BLD510B.h"
#include "ModBusBuilder.h"

// Define the RE/DE pin used for RS485 direction control
static const uint8_t RS485_RTS_PIN = 5;

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

    // Speed register expects Low Byte first according to BLD-510B manual
    uint8_t data1 = RPM & 0xFF;
    uint8_t data2 = (RPM >> 8) & 0xFF;

    size_t len = mbBuilder.buildWriteSingleRegister(
        this->addr,
        0x8005,
        data1,
        data2,
        frame
    );

    while (Serial1.available())
    {
        Serial1.read();
    }

    digitalWrite(RS485_RTS_PIN, HIGH);
    Serial1.write(frame, len);
    Serial1.flush();
    digitalWrite(RS485_RTS_PIN, LOW);

    unsigned long startTime = millis();
    while (Serial1.available() < 8)
    {
        if (millis() - startTime > 50) 
        { 
            break; 
        }
    }

    while (Serial1.available())
    {
        Serial1.read();
    }

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

    while (Serial1.available())
    {
        Serial1.read();
    }

    digitalWrite(RS485_RTS_PIN, HIGH);
    Serial1.write(frame, len);
    Serial1.flush();
    digitalWrite(RS485_RTS_PIN, LOW);

    unsigned long startTime = millis();
    while (Serial1.available() < 8)
    {
        if (millis() - startTime > 50) 
        { 
            break; 
        }
    }

    while (Serial1.available())
    {
        Serial1.read();
    }

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

    while (Serial1.available())
    {
        Serial1.read();
    }

    digitalWrite(RS485_RTS_PIN, HIGH);
    Serial1.write(frame, len);
    Serial1.flush();
    digitalWrite(RS485_RTS_PIN, LOW);

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

    if (received != 7 || 
        response[0] != this->addr || 
        response[1] != 0x03 || 
        response[2] != 2)
    {
        return 0;
    }

    uint16_t speed = ((uint16_t)response[3] << 8) | response[4];

    return speed;
}
