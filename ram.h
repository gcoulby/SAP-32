#ifndef RAM_H
#define RAM_H
#include "register.h"
#include "bus.h"
#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

enum PROGRAM_MODE { MAN, AUTO };

enum ASM {
    NOP = 0b0000,
    LDA = 0b0001,
    ADD = 0b0010,
    SUB = 0b0011,
    STA = 0b0100,
    LDI = 0b0101,
    JMP = 0b0110,
    JC  = 0b0111,
    JZ  = 0b1000,
    //...//
    OUT = 0b1110,
    HLT = 0b1111
};

uint8_t asm2bin(ASM instruction, uint16_t *addr);

class Ram
{
    private:
        uint8_t memoryBank[16] = {0};
        Bus &bus;
        Register &mar;
        Adafruit_MCP23X17 &mcp;
    public:
        bool RamOut;
        bool RamIn;
        uint8_t currentRam;
        PROGRAM_MODE programMode;
        Ram(Bus &bus, Register &mar, Adafruit_MCP23X17 &mcp);
        void onRisingClock();
        void handleInput();
        void handleOutput();
        void Program();
        int ReadMux(int channel);
        void StoreAddressFromMux();
        void StoreMemoryFromMuxAtAddressFromMux();
        void InitialiseCtrlBits();
        void TestCode();
};


#endif