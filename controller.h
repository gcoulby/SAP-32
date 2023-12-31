#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "alu.h"
#include "bus.h"
// #include "clock.h"
#include "program_counter.h"
#include "ram.h"
#include "register.h"
#include "eeprom.h"
#include "led_matrix.h"

class Controller
{
    private:
        uint16_t ControlWord; //16 bit control word
        uint8_t controlStep;
        uint8_t maxControlStepCount;

        Adafruit_MCP23X17 &mcp;
        EEPROM* eeprom;
        Bus* bus;
        ProgramCounter* programCounter;
        AluRegister* a_reg;
        AluRegister* b_reg;
        InstructionRegister* ir;
        MemoryAddressRegister* mar;
        OutputRegister* out_reg;
        FlagRegister* flagRegister;
        Alu* alu;
        Ram* ram;
        LedMatrix* ledMatrix;
        int LastClockState;
        uint8_t lastRamValue;
        uint8_t lastAddress;

        void incrementControlStep();
        void onRisingClock(bool skipIncrement = false);
        void InitialiseCtrlBits();
        void handleOutputs();
        void handleInputs();
        void Reset();
        void ClearData();
        void debug();
        void handleInstruction(uint16_t instruction);
    public:
        Controller(Adafruit_MCP23X17 &mcp);
        void handleEmulationCycle();
};

#endif