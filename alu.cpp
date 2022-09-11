#include "alu.h"
#include "bus.h"

Alu::Alu(AluRegister &a, AluRegister &b, Bus &bus): a(a), b(b), bus(bus) {
    this->sub = 0;
    this->sigOut = 0; 
}

void Alu::sum() {
    uint8_t a = this->a.DATA;
    uint8_t b = this->b.DATA;
    this->result = (sub == 0) ? (uint8_t) (a + b) : (int8_t) (a - b);
}


void Alu::onRisingClock(){
    sum();
    if(this->sigOut == 1) {
        this->bus.SetData(this->result);
        this->sigOut = 0;
    }
}