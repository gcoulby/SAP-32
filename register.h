#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include "bus.h"

class Register
{
    public:
        uint8_t InputEnable;      
        uint8_t CLR;
        uint8_t DATA;
    protected:
        void handleRisingClock();
        void handleInput();
        void handleClear();
        Register(Bus &bus);
        Bus &bus;
};

class AluRegister : public Register
{
    public:
        uint8_t OutputEnable;
        void handleOutput();
        AluRegister(Bus &bus);
        void onRisingClock();
};

class InstructionRegister : public Register
{
    public:
        uint8_t OutputEnable;
        void handleOutput();
        InstructionRegister(Bus &bus);
        void onRisingClock();
};

class MemoryAddressRegister : public Register
{
    public:
        MemoryAddressRegister(Bus &bus);
        void onRisingClock();
};

class OutputRegister : public Register
{
    public:
        OutputRegister(Bus &bus);
        void onRisingClock();
};




#endif