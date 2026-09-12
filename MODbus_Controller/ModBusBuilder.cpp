#include "ModBusBuilder.h"


ModBusBuilder mbBuilder;


uint16_t ModBusBuilder::modbusCRC(
    const uint8_t *data,
    size_t length
)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];

        for (int bit = 0; bit < 8; ++bit)
        {
            if (crc & 1)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}


size_t ModBusBuilder::buildWriteSingleRegister(
    uint8_t server,
    uint16_t address,
    uint8_t data1,
    uint8_t data2,
    uint8_t *frame
)
{
    frame[0] = server;
    frame[1] = 0x06;

    frame[2] = (address >> 8) & 0xFF;
    frame[3] = address & 0xFF;

    frame[4] = data1;
    frame[5] = data2;

    uint16_t crc = modbusCRC(frame, 6);

    frame[6] = crc & 0xFF;
    frame[7] = (crc >> 8) & 0xFF;

    return 8;
}


size_t ModBusBuilder::buildReadRegisters(
    uint8_t server,
    uint16_t address,
    uint16_t quantity,
    uint8_t *frame
)
{
    frame[0] = server;
    frame[1] = 0x03;

    frame[2] = (address >> 8) & 0xFF;
    frame[3] = address & 0xFF;

    frame[4] = (quantity >> 8) & 0xFF;
    frame[5] = quantity & 0xFF;

    uint16_t crc = modbusCRC(frame, 6);

    frame[6] = crc & 0xFF;
    frame[7] = (crc >> 8) & 0xFF;

    return 8;
}
