#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include "bus.h"
#include <TM1637Display.h>

extern bool CARRY;
extern bool ZERO;

class Register
{
    public:
        uint8_t InputEnable;      
        // uint8_t CLR;
        uint8_t DATA;
        void handleInput();
        void CLR();
    protected:
        void handleInitialiseCtrlBits();
        Register(Bus &bus);
        Bus &bus;
};

class AluRegister : public Register
{
    public:
        uint8_t OutputEnable;
        void handleOutput();
        AluRegister(Bus &bus);
        void InitialiseCtrlBits();
};

class InstructionRegister : public Register
{
    public:
        uint8_t OutputEnable;
        void handleOutput();
        InstructionRegister(Bus &bus);
        void InitialiseCtrlBits();
        uint8_t fetchInstructionCode();
};

class MemoryAddressRegister : public Register
{
    public:
        MemoryAddressRegister(Bus &bus);
        void InitialiseCtrlBits();
};

class OutputRegister : public Register
{
    private:
        TM1637Display display;
    public:
        OutputRegister(Bus &bus);
        void InitialiseCtrlBits();
        void UpdateDisplay();
        
};

class FlagRegister : public Register
{
    private:
        AluRegister &a;
        AluRegister &b;
    public:
        bool carry;
        bool zero;
        bool carrySet();
        bool zeroSet();
        FlagRegister(Bus &bus, AluRegister &a, AluRegister &b);
        void setFlags(bool carry, bool zero); 
        void setFlags();
        void ResetState();
};


#endif