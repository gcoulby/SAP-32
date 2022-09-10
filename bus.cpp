#include "bus.h"

Bus::Bus() {
    this->DATA = 0b01101110;
}

void Bus::onRisingClock() {
    //
}

uint8_t Bus::GetData(ORDER order) {
    switch (order) {
        case BYTE:
            return (uint8_t) this->DATA;
        case LOW_ORDER:
            return (uint8_t) lowNibble(this->DATA);
        case HIGH_ORDER:
            return (uint8_t) highNibble(this->DATA);
    }
}

void Bus::SetData(uint8_t data, ORDER order) {
    switch (order) {
        case BYTE:
            this->DATA = data;
            break;
        case LOW_ORDER:
            this->DATA = lowNibble(data);
            break;
        case HIGH_ORDER:
            this->DATA = highNibble(data) << 4;
            break;
    }
}



