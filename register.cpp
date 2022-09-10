#include "register.h"
#include "bus.h"

Register::Register(REG_TYPE type, Bus& bus) : bus(bus) {
  this->DATA = 0;
  this->LATCH = 0;
  this->ENABLE = 0;
  this->type = type;
}

void Register::onRisingClock(){
    if(this->LATCH == 1) {   
        this->DATA = bus.GetData();
        this->LATCH = 0;
    }
    if(this->ENABLE == 1) { 
        if(this->type == INS_REG){
            // uint8_t addr = lowNibble(this->DATA);
            this->bus.SetData(this->DATA, LOW_ORDER);

            //inst to be sent to instruction decoder
            // uint8_t inst = highNibble(this->DATA);
        }
        else{

            this->bus.SetData(this->DATA);
        }
        this->ENABLE = 0;
    }
    if(this->CLR == 1) {
        this->DATA = 0;
        this->CLR = 0;
    }
}

