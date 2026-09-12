#ifndef MODBUSBUILDER_H
#define MODBUSBUILDER_H

#include <Arduino.h>

class ModBusBuilder
{
public:
    uint16_t modbusCRC(const uint8_t *data, size_t length);

    size_t buildWriteSingleRegister(
        uint8_t server,
        uint16_t address,
        uint8_t data1,
        uint8_t data2,
        uint8_t *frame
    );

    size_t buildReadRegisters(
        uint8_t server,
        uint16_t address,
        uint16_t quantity,
        uint8_t *frame
    );
};

extern ModBusBuilder mbBuilder;

#endif
