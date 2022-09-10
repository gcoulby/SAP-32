#include "program_counter.h"

#include "bus.h"

ProgramCounter::ProgramCounter(Bus &bus) : bus(bus) {
    maxValue = 0b1111;
    Count = 0;
    CounterEnable = 1;
    CounterOut = 0;
    Jump = 0;
}

void ProgramCounter::onRisingClock(){
    if(Jump){
        Count = bus.GetData(LOW_ORDER);
    }
    if(CounterEnable){
        this->increment();
    }
    if(CounterOut){
        this->bus.SetData(this->Count, LOW_ORDER);
    }
}

void ProgramCounter::increment(){
    if(Count == maxValue){
        this->Count = 0;
    }
    else{
        this->Count++;
    }
}