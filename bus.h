#ifndef BUS_H  
#define BUS_H

#include <stdint.h>
#include <Arduino.h>

#define lowNibble(w) ((uint8_t) ((w) & 0xf))
#define highNibble(w) ((uint8_t) ((w) >> 4))

enum ORDER { BYTE, LOW_ORDER, HIGH_ORDER };

class Bus
{
    public:
        uint8_t GetData(ORDER order = BYTE);
        void SetData(uint8_t data, ORDER order = BYTE);
        Bus();
        void InitialiseCtrlBits();
    private:
        uint8_t DATA;

};

#endif