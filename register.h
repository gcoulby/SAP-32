#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include "bus.h"

enum REG_TYPE { ALU_REG, MAR_REG, INS_REG, OUT_REG };

class Register
{
    public:
        uint8_t LATCH;
        uint8_t ENABLE;
        uint8_t CLR;
        REG_TYPE type;
        uint8_t DATA;
        Register(REG_TYPE type, Bus &bus);
        void onRisingClock();
    private:
        Bus &bus;
};



#endif