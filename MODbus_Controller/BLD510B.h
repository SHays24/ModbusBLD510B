#ifndef BLD510B_H
#define BLD510B_H

#include <Arduino.h>

class BLD510B
{
private:
    uint8_t addr;
    bool motorDirection;

public:

    BLD510B(
        uint8_t server,
        bool motorDir
    );

    bool setSpeed(
        uint16_t RPM
    );

    bool setControl(
        uint8_t control,
        uint8_t polePairs
    );

    uint16_t getSpeed();
};

#endif
