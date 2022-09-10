#ifndef BUS_H  
#define BUS_H

#include <stdint.h>
#include <Arduino.h>

class Bus
{
    public:
        uint8_t GetData();
        void SetData(uint8_t data);
        Bus();
        void onRisingClock();
    private:
        uint8_t DATA;

};

#endif