#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include "bus.h"

class ProgramCounter{
    public:
        uint8_t Count;
        bool Jump;
        bool CounterEnable;
        bool CounterOut;
        ProgramCounter(Bus &bus);
        void onRisingClock();
        void increment();
    private:
        Bus &bus;
        uint8_t maxValue;
};

#endif