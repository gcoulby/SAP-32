#ifndef ALU_H
#define ALU_H

#include "register.h"
#include "bus.h"

class Alu
{
    public:
        Alu(AluRegister &a, AluRegister &b, Bus &bus);
        int result;
        bool sigOut;
        bool sub;
        void onRisingClock();
    private:
        AluRegister &a;
        AluRegister &b;
        Bus &bus;
        void sum();
};

#endif