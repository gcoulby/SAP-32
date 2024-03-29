#ifndef ALU_H
#define ALU_H

#include "register.h"
#include "bus.h"

class Alu
{
    public:
        Alu(AluRegister &a, AluRegister &b, FlagRegister &f, Bus &bus);
        int result;
        bool sigOut;
        bool sub;
        // bool carry;
        // bool zero;
        void handleOutput();
        void sum();
        // void storeFlags();
        void InitialiseCtrlBits();
    private:
        AluRegister &a;
        AluRegister &b;
        FlagRegister &f;
        Bus &bus;
};

#endif