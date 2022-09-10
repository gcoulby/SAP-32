#ifndef ALU_H
#define ALU_H

#include "register.h"
#include "bus.h"

class Alu
{
    public:
        Alu(Register &a, Register &b, Bus &bus);
        int result;
        bool sigOut;
        bool sub;
        void onRisingClock();
    private:
        Register &a;
        Register &b;
        Bus &bus;
        void sum();
};

#endif