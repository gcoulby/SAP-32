#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "LedController.hpp"
#include "alu.h"
#include "bus.h"
// #include "clock.h"
#include "program_counter.h"
#include "ram.h"
#include "register.h"
#include "eeprom.h"
#include "led_matrix.h"

//the total number of segments.
//In this example they are in one row and you can freely change this number.
#define LedSegments 2

class LedMatrix
{
    public:
        LedMatrix(Bus &bus, EEPROM &eeprom, ProgramCounter &programCounter, AluRegister &a_reg, AluRegister &b_reg, InstructionRegister &ir, MemoryAddressRegister &mar, FlagRegister &flagRegister, Alu &alu, Ram &ram, uint8_t *controlStep, uint16_t *controlWord);
        void UpdateDisplay();
    private:
        uint8_t delayTime;
        LedController<LedSegments,1> lc;
        void setRow(uint8_t segment, uint8_t row, uint8_t value, bool reverse = false);
        Bus &bus;
        EEPROM &eeprom;
        ProgramCounter &programCounter;
        AluRegister &a_reg;
        AluRegister &b_reg;
        InstructionRegister &ir;
        MemoryAddressRegister &mar;
        FlagRegister &flagRegister;
        Alu &alu;
        Ram &ram;
        uint8_t *controlStep;
        uint16_t *controlWord;
};

#endif