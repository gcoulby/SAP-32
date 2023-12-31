#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include "bus.h"

class ProgramCounter{
    public:
        uint8_t Count;
        bool CounterEnable;
        bool CounterOut;
        ProgramCounter(Bus &bus);
        void Jump();
        void handleInput();
        void handleOutput();
        void increment();
        void InitialiseCtrlBits();

    private:
        Bus &bus;
        uint8_t maxValue;
};

#endif