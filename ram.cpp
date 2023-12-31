#include "ram.h"
#include "bus.h"
#include <Arduino.h>

const int ADDRESS_SIZE = 4;
const int DATA_SIZE = 8;

int memoryLoadPin = 17;
int addressSelectPin = 16;

// int s0 = 26;
// int s1 = 27;
// int s2 = 14;
// int s3 = 12;
// int SIG_pin = 13;

int address_channels[4] = {11, 10, 9, 8};
int data_channels[8] = {7,6,5,4,3,2,1};


Ram::Ram(Bus &bus, Register &mar, Adafruit_MCP23X17 &mcp) : bus(bus), mar(mar), mcp(mcp) {
    this->RamIn = 0;
    this->RamOut = 0;
    this->currentRam = 0;
    this->programMode = AUTO;


    // pinMode(s0, OUTPUT);
    // pinMode(s1, OUTPUT);
    // pinMode(s2, OUTPUT);
    // pinMode(s3, OUTPUT);
    // pinMode(SIG_pin, INPUT);
    
    // digitalWrite(s0, LOW);
    // digitalWrite(s1, LOW);
    // digitalWrite(s2, LOW);
    // digitalWrite(s3, LOW);


    for(int i = 0; i < ADDRESS_SIZE + DATA_SIZE; i++){
        mcp.pinMode(i, INPUT_PULLUP);
    }
    
    pinMode(memoryLoadPin, INPUT_PULLDOWN);
    pinMode(addressSelectPin, INPUT_PULLDOWN);

    TestCode();
}

void Ram::StoreAddressFromMux(){
    this->mar.DATA = 0;
    for (int i = 0; i < ADDRESS_SIZE; i++)
    {
        int a = mcp.digitalRead(address_channels[i]) == 1; 
        int b =  a << i;
        this->mar.DATA += b;
    }
    this->currentRam = this->memoryBank[this->mar.DATA];
}

void Ram::StoreMemoryFromMuxAtAddressFromMux(){
    this->mar.DATA = 0;


    for (int i = 0; i < ADDRESS_SIZE; i++)
    {
        int a = mcp.digitalRead(address_channels[i]) == 1; 
        int b =  a << i;
        this->mar.DATA += b;
    }

    int o = 0;
    for (int i = 0; i < DATA_SIZE; i++)
    {
        int a = mcp.digitalRead(data_channels[i]) == 1; 
        int b =  a << i;
        o += b;
    }
    this->memoryBank[this->mar.DATA] = o;
    this->currentRam = o;
}

void Ram::Program(){
        if(digitalRead(addressSelectPin) == HIGH){
            this->StoreAddressFromMux();
            
        }
        if(digitalRead(memoryLoadPin) == HIGH){
            this->StoreMemoryFromMuxAtAddressFromMux();
        }

        delayMicroseconds(200);
}

// int Ram::ReadMux(int channel){
//   int controlPin[] = {s0, s1, s2, s3};

//   int muxChannel[16][4]={
//     {0,0,0,0}, //channel 0
//     {1,0,0,0}, //channel 1
//     {0,1,0,0}, //channel 2
//     {1,1,0,0}, //channel 3
//     {0,0,1,0}, //channel 4
//     {1,0,1,0}, //channel 5
//     {0,1,1,0}, //channel 6
//     {1,1,1,0}, //channel 7
//     {0,0,0,1}, //channel 8
//     {1,0,0,1}, //channel 9
//     {0,1,0,1}, //channel 10
//     {1,1,0,1}, //channel 11
//     {0,0,1,1}, //channel 12
//     {1,0,1,1}, //channel 13
//     {0,1,1,1}, //channel 14
//     {1,1,1,1}  //channel 15
//   };

//   //loop through the 4 sig
//   for(int i = 0; i < 4; i ++){
//     digitalWrite(controlPin[i], muxChannel[channel][i]);
//   }

//   //read the value at the SIG pin
//   int val = analogRead(SIG_pin);

//   //return the value
//   return val;
// }

void Ram::handleInput(){
    this->currentRam = this->memoryBank[this->mar.DATA];
    if(this->RamIn ==  1) {
        this->memoryBank[this->mar.DATA] = this->bus.GetData();
        this->currentRam = this->bus.GetData();
    }
}

void Ram::handleOutput(){
    this->currentRam = this->memoryBank[this->mar.DATA];
    if(this->RamOut == 1) {
        this->bus.SetData(this->memoryBank[this->mar.DATA]);
    }
}

void Ram::InitialiseCtrlBits(){
    this->RamIn = 0;
    this->RamOut = 0;
}

uint8_t asm2bin(ASM instruction, uint8_t addr){
    uint8_t o = 0;
    return (((uint8_t) instruction) << 4) + addr;
}


void Ram::TestCode(){
    // this->memoryBank[0] = 0b11100000; // OUT
    // this->memoryBank[1] = 0b00101111; // ADD 15
    // this->memoryBank[2] = 0b01110100; // JC 4
    // this->memoryBank[3] = 0b01100000; // JMP 0
    // this->memoryBank[4] = 0b11100000; // OUT
    // this->memoryBank[5] = 0b00111111; // SUB 15
    // this->memoryBank[6] = 0b11100000; // OUT
    // this->memoryBank[7] = 0b10000000; // JZ 0
    // this->memoryBank[8] = 0b01100100; // JMP 4
    // this->memoryBank[9] = 0b00000000;
    // this->memoryBank[10] = 0b00000000;
    // this->memoryBank[11] = 0b00000000;
    // this->memoryBank[12] = 0b00000000;
    // this->memoryBank[13] = 0b00000000;
    // this->memoryBank[14] = 0b00000000;
    // this->memoryBank[15] = 0b00001010;

    // /*
    //     Add 10 in loop but reverse when 255 is reached
    // */
    // this->memoryBank[0] = asm2bin(ASM::OUT, (uint8_t) 0); // OUT
    // this->memoryBank[1] = asm2bin(ASM::ADD, 15); // ADD 15
    // this->memoryBank[2] = asm2bin(ASM::JC, 4); // JC 4
    // this->memoryBank[3] = asm2bin(ASM::JMP, (uint8_t) 0); // JMP 0
    // this->memoryBank[4] = asm2bin(ASM::OUT, (uint8_t) 0); // OUT
    // this->memoryBank[5] = asm2bin(ASM::SUB, 15); // SUB 15
    // this->memoryBank[6] = asm2bin(ASM::OUT, (uint8_t) 0); // OUT
    // this->memoryBank[7] = asm2bin(ASM::JZ, (uint8_t) 0); // JZ 0
    // this->memoryBank[8] = asm2bin(ASM::JMP, 4); // JMP 4
    // this->memoryBank[9] = asm2bin(ASM::NOP, (uint8_t) 0);
    // this->memoryBank[10] = asm2bin(ASM::NOP,(uint8_t) 0);
    // this->memoryBank[11] = asm2bin(ASM::NOP,(uint8_t) 0);
    // this->memoryBank[12] = asm2bin(ASM::NOP,(uint8_t) 0);
    // this->memoryBank[13] = asm2bin(ASM::NOP,(uint8_t) 0);
    // this->memoryBank[14] = asm2bin(ASM::NOP,(uint8_t) 0);
    // this->memoryBank[15] = asm2bin(ASM::NOP, 10);

    /*
        Multiply 3 by 5

    */
    this->memoryBank[0] = asm2bin(ASM::LDA, (uint8_t) 14); // LDA 14
    this->memoryBank[1] = asm2bin(ASM::SUB, (uint8_t) 12); // SUB 12
    this->memoryBank[2] = asm2bin(ASM::JC, (uint8_t) 6); // JC 6
    this->memoryBank[3] = asm2bin(ASM::LDA, (uint8_t) 13); // LDA 13
    this->memoryBank[4] = asm2bin(ASM::OUT, (uint8_t) 0); // OUT
    this->memoryBank[5] = asm2bin(ASM::HLT, (uint8_t) 0); // HLT
    this->memoryBank[6] = asm2bin(ASM::STA, (uint8_t) 14); // STA 14
    this->memoryBank[7] = asm2bin(ASM::LDA, (uint8_t) 13); // LDA 13
    this->memoryBank[8] = asm2bin(ASM::ADD, (uint8_t) 15); // ADD 15
    this->memoryBank[9] = asm2bin(ASM::STA, (uint8_t) 13); // STA 13
    this->memoryBank[10] = asm2bin(ASM::JMP, (uint8_t) 0); // JMP 0
    this->memoryBank[11] = asm2bin(ASM::NOP, (uint8_t) 0);
    this->memoryBank[12] = asm2bin(ASM::NOP, (uint8_t) 1);
    this->memoryBank[13] = asm2bin(ASM::NOP, (uint8_t) 0);
    this->memoryBank[14] = asm2bin(ASM::NOP, (uint8_t) 3);
    this->memoryBank[15] = asm2bin(ASM::NOP, (uint8_t) 5);


    // this->memoryBank[2] = 0b11100000; // OUT
    // this->memoryBank[3] = 0b00101111; // ADD 15
    // this->memoryBank[4] = 0b11100000; // OUT
    // this->memoryBank[5] = 0b00101111; // ADD 15
    // this->memoryBank[6] = 0b11100000; // OUT
    // this->memoryBank[7] = 0b00101111; // ADD 15
    // this->memoryBank[8] = 0b11100000; // OUT
    // this->memoryBank[9] = 0b00101111; // ADD 15
    // this->memoryBank[10] = 0b11100000; // OUT
    // this->memoryBank[11] = 0b11110000; // HLT
    // this->memoryBank[12] = 0b00000000;
    // this->memoryBank[13] = 0b00000000;
    // this->memoryBank[14] = 0b00000000;
    // this->memoryBank[15] = 0b00001010;
}