#include "bus.h"

Bus::Bus() {
    this->DATA = 0b01101110;
}

void Bus::onRisingClock() {
    //
}

uint8_t Bus::GetData() {
    return (uint8_t) this->DATA;
}

void Bus::SetData(uint8_t data) {
    this->DATA = (uint8_t) data;
}



