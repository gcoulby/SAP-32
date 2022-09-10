#ifndef RAM_H
#define RAM_H
#include "register.h"
#include "bus.h"
#include <Arduino.h>

enum PROGRAM_MODE { MAN, AUTO };

class Ram
{
    private:
        uint8_t memoryBank[16][8] = {{0}};
        Bus &bus;
        Register &mar;
    public:
        bool RamOut;
        bool RamIn;
        // uint8_t address;
        uint8_t currentRam;
        PROGRAM_MODE programMode;
        Ram(Bus &bus, Register &mar);
        void onRisingClock();
        void Program();
        int ReadMux(int channel);
        void StoreAddressFromMux();
        void StoreMemoryFromMuxAtAddressFromMux();
};


#endif