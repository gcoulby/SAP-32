#include "register.h"
#include "bus.h"

Register::Register(Bus& bus) : bus(bus) {
  this->DATA = 0;
  this->InputEnable = 0;
}

void Register::handleRisingClock(){
    this->handleInput();
    this->handleClear();
}

void Register::handleInput(){
    if(this->InputEnable == 1) {   
        this->DATA = bus.GetData();
        this->InputEnable = 0;
    }
}

void Register::handleClear(){
    if(this->CLR == 1) {
        this->DATA = 0;
        this->CLR = 0;
    }
}

/*=========================AluRegister=========================*/

AluRegister::AluRegister(Bus& bus) : Register(bus) {
    this->OutputEnable = 0;
}

void AluRegister::onRisingClock(){
    this->handleRisingClock();
    this->handleOutput();
}

void AluRegister::handleOutput(){
    if(this->OutputEnable == 1) {
        this->bus.SetData(this->DATA);
        this->OutputEnable = 0;
    }
}

/*=========================InstructionRegister=========================*/

InstructionRegister::InstructionRegister(Bus& bus) : Register(bus) {
    this->OutputEnable = 0;
}

void InstructionRegister::onRisingClock(){
    this->handleRisingClock();
    this->handleOutput();
}

void InstructionRegister::handleOutput(){
    if(this->OutputEnable == 1) {
        uint8_t addr = lowNibble(this->DATA);
        this->bus.SetData(addr);
        this->OutputEnable = 0;
    }
}


/*=========================MemoryAddressRegister=========================*/

MemoryAddressRegister::MemoryAddressRegister(Bus& bus) : Register(bus) {
}

void MemoryAddressRegister::onRisingClock() {
    this->handleRisingClock();
}

/*=========================OutputRegister=========================*/

OutputRegister::OutputRegister(Bus& bus) : Register(bus) {
}

void OutputRegister::onRisingClock(){
    this->handleRisingClock();
}

/*=========================FlagRegister=========================*/
FlagRegister::FlagRegister(Bus& bus) : Register(bus) {
    bool zero;
    bool carry;
}



void FlagRegister::setFlags(bool carry, bool zero) {
    if(this->InputEnable == 1) {
        this->DATA = 0;
        this->DATA |= carry << 1;
        this->DATA |= zero << 0;
        // this->zero = this->alu.zero;
        // this->carry = this->alu.carry;
        this->InputEnable = 0;
    }
}

