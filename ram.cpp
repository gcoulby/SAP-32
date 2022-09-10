#include "ram.h"
#include "bus.h"
#include <Arduino.h>

const int ADDRESS_SIZE = 4;
const int DATA_SIZE = 8;

int memoryLoadPin = 14;
int addressSelectPin = 27;

int s0 = 23;
int s1 = 18;
int s2 = 5;
int s3 = 17;
int SIG_pin = A1;

int address_channels[4] = {8, 9, 10, 11};
int data_channels[8] = {0, 1, 2, 3, 4, 5, 6, 7};


Ram::Ram(Bus &bus, Register &mar) : bus(bus), mar(mar) {
    this->RamIn = 0;
    this->RamOut = 0;
    this->currentRam = 0;
    this->programMode = AUTO;

    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(SIG_pin, INPUT);
    
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    
    pinMode(memoryLoadPin, INPUT_PULLDOWN);
    pinMode(addressSelectPin, INPUT_PULLDOWN);
}

void Ram::StoreAddressFromMux(){
    this->mar.DATA = 0;
    for (int i = 0; i < ADDRESS_SIZE; i++)
    {
        int a = this->ReadMux(address_channels[i]) > 10; 
        int b =  a << i;
        this->mar.DATA += b;
    }
    this->currentRam = this->memoryBank[this->mar.DATA];
}

void Ram::StoreMemoryFromMuxAtAddressFromMux(){
    this->mar.DATA = 0;


    for (int i = 0; i < ADDRESS_SIZE; i++)
    {
        int a = this->ReadMux(address_channels[i]) > 10; 
        int b =  a << i;
        this->mar.DATA += b;
    }

    int o = 0;
    for (int i = 0; i < DATA_SIZE; i++)
    {
        int a = this->ReadMux(data_channels[i]) > 10; 
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

        delayMicroseconds(20);
}

int Ram::ReadMux(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pin);

  //return the value
  return val;
}

void Ram::onRisingClock() {
    if(this->RamIn ==  1) {
        this->memoryBank[this->mar.DATA] = this->bus.GetData();
        this->currentRam = this->bus.GetData();
        this->RamIn = 0;
    }
    if(this->RamOut == 1) {
        this->bus.SetData(this->memoryBank[this->mar.DATA]);
        this->RamOut = 0;
    }


}