#include "alu.h"
#include "bus.h"

Alu::Alu(AluRegister &a, AluRegister &b, FlagRegister &f, Bus &bus): a(a), b(b), f(f), bus(bus) {
    this->sub = 0;
    this->sigOut = 0; 
    // this->carry = 0;
    // this->zero = 0;
}

void Alu::sum() {
    uint8_t a = this->a.DATA;
    uint8_t b = this->b.DATA;

    if(this->sub == 0) {
        uint8_t result = a + b;
        ZERO = result == 0;
        // CARRY = a >= b && result < a || b >= a && result < b;//  (int) a + (int) b > 0b11111111;
        CARRY = (int) a + (int) b > 0b11111111;
        // CARRY = true;
        // this->carry = true;
        // this->zero = true;
        this->result = result;
    }
    else {
        CARRY = ((int)a - (int)b < 0);
        uint8_t result = a - b;
        ZERO = result == 0;
        this->result = result;
    }
    this->f.setFlags();
}

// void Alu::storeFlags(){

//     if(f.InputEnable){
//         this->f.setFlags(this->carry, this->zero);
//     }
// }

void Alu::handleOutput(){
    if(this->sigOut == 1) {
        this->bus.SetData(this->result);

        /// IS THIS THE BUG? TODO
        // ZERO = this->result == 0;
        // CARRY = true;


        // this->sigOut = 0;
    }
}

void Alu::InitialiseCtrlBits(){
    // this->storeFlags();
    // this->carry=0;
    // this->zero=0;
    // CARRY = 0;
    // ZERO = 0;
    ZERO = this->result == 0;
    CARRY = this->a.DATA >= this->b.DATA && this->result < this->a.DATA || this->b.DATA >= this->a.DATA && this->result < this->b.DATA;//  (int) a + (int) b > 0b11111111;
    this->sub = 0;
    this->sigOut = 0; 
}