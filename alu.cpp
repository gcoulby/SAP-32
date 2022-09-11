#include "alu.h"
#include "bus.h"

Alu::Alu(AluRegister &a, AluRegister &b, FlagRegister &f, Bus &bus): a(a), b(b), f(f), bus(bus) {
    this->sub = 0;
    this->sigOut = 0; 
    this->carry = 0;
    this->zero = 0;
}

void Alu::sum() {
    uint8_t a = this->a.DATA;
    uint8_t b = this->b.DATA;

    if(this->sub == 0) {
        this->carry = a + b > 0b11111111;
        this->zero = a + b == 0;
        uint8_t result = a + b;
        this->result = result;
    }
    else {
        this->carry = (a - b < 0);
        this->zero = a - b == 0;
        int8_t result = a - b;
        this->result = result;
    }
}


void Alu::onRisingClock(){
    this->f.setFlags(this->carry, this->zero);
    sum();
    if(this->sigOut == 1) {
        this->bus.SetData(this->result);
        this->sigOut = 0;
    }
}