#include "register.h"
#include "bus.h"
#include <TM1637Display.h>

bool CARRY = 0;
bool ZERO = 0;


Register::Register(Bus& bus) : bus(bus) {
  this->DATA = 0;
  this->InputEnable = 0;
}


void Register::handleInput(){
    if(this->InputEnable == 1) {   
        this->DATA = bus.GetData();
    }
}

void Register::CLR(){
    this->DATA = 0;
}

void Register::handleInitialiseCtrlBits(){
    this->InputEnable = 0;
    // this->CLR = 0;
}

/*=========================AluRegister=========================*/

AluRegister::AluRegister(Bus& bus) : Register(bus) {
    this->OutputEnable = 0;
}

void AluRegister::handleOutput(){
    if(this->OutputEnable == 1) {
        this->bus.SetData(this->DATA);
    }
}

void AluRegister::InitialiseCtrlBits(){
    this->handleInitialiseCtrlBits();
    this->OutputEnable = 0;
}

/*=========================InstructionRegister=========================*/

InstructionRegister::InstructionRegister(Bus& bus) : Register(bus) {
    this->OutputEnable = 0;
}

void InstructionRegister::handleOutput(){
    if(this->OutputEnable == 1) {
        uint8_t addr = lowNibble(this->DATA);
        this->bus.SetData(addr);
    }
}

uint8_t InstructionRegister::fetchInstructionCode(){
    return highNibble(this->DATA);
}

void InstructionRegister::InitialiseCtrlBits(){
    this->handleInitialiseCtrlBits();
    this->OutputEnable = 0;
}


/*=========================MemoryAddressRegister=========================*/

MemoryAddressRegister::MemoryAddressRegister(Bus& bus) : Register(bus) {
}

void MemoryAddressRegister::InitialiseCtrlBits() {
    this->handleInitialiseCtrlBits();
}

/*=========================OutputRegister=========================*/


int sevenSegCLK = 26;
int sevenSegDIO = 25;
OutputRegister::OutputRegister(Bus& bus) : Register(bus), display(sevenSegCLK, sevenSegDIO) {
    this->display = TM1637Display(sevenSegCLK, sevenSegDIO);
    this->display.clear();
    this->display.setBrightness(0x0a);
    this->display.showNumberDec(0, false);
}


void OutputRegister::InitialiseCtrlBits(){
    this->handleInitialiseCtrlBits();
}

void OutputRegister::UpdateDisplay(){
    // this->display.clear();
    // Serial.println((int)this->DATA);
    this->display.showNumberDec((int)this->DATA);
}

/*=========================FlagRegister=========================*/
FlagRegister::FlagRegister(Bus& bus, AluRegister &a, AluRegister &b) : Register(bus), a(a), b(b) {
    bool zero = 0;
    bool carry = 0;
}

void FlagRegister::setFlags() {
    if(this->InputEnable == 1) {
        this->DATA = 0;
        if(CARRY == true) {
            this->DATA |= 0b0010;
        }
        if(ZERO == true) {
            this->DATA |= 0b0001;
        }
    }
}

bool FlagRegister::carrySet() {
    return this->DATA & 0b0010;
}

bool FlagRegister::zeroSet() {
    return this->DATA & 0b0001;
}

void FlagRegister::ResetState(){
    this->handleInitialiseCtrlBits();
}
