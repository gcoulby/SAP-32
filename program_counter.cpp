#include "program_counter.h"

#include "bus.h"

ProgramCounter::ProgramCounter(Bus &bus) : bus(bus) {
    maxValue = 0b1111;
    Count = 0;
    CounterEnable = 0;
    CounterOut = 0;
    // Jump = 0;
}

void ProgramCounter::Jump(){
    Count = bus.GetData(LOW_ORDER);
}

void ProgramCounter::handleInput() {
    // if(Jump){
        // Count = bus.GetData(LOW_ORDER);
    // }
    if (this->CounterEnable == 1) {
        this->increment();
    }
}

void ProgramCounter::handleOutput() {
    if (this->CounterOut == 1) {
        this->bus.SetData(this->Count);
    }
}

void ProgramCounter::increment(){
    if(Count == maxValue){
        this->Count = 0;
        Serial.println("RESET");
    }
    else{
        this->Count++;
    }
}

void ProgramCounter::InitialiseCtrlBits(){
    CounterEnable = 0;
    CounterOut = 0;
    // Jump = 0;
}