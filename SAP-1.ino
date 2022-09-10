/*=================================*/
/*         Includes
/*=================================*/
#include "clock.h"
#include "register.h"
#include "globals.h"
#include "alu.h"
#include "ram.h"
#include "bus.h"

#define DEBUG 1

/*=================================*/
/*         Global Variables        
/*=================================*/

int counterResetPin = 22;
int programModePin = 15;
int programModeLedPin = 2;


int LastClockState = 0;
Bus* bus = new Bus();
Register* a_reg = new Register(ALU_REG, *bus);
Register* b_reg = new Register(ALU_REG, *bus);
Register* ir = new Register(INS_REG, *bus);
Register* mar = new Register(MAR_REG, *bus);
Alu* alu = new Alu(*a_reg, *b_reg, *bus);
Ram* ram = new Ram(*bus, *mar);



/*=================================*/
/*       Setup                  
/*=================================*/


void setup() {
  Serial.begin(500000);
  pinMode(counterResetPin, INPUT);
  pinMode(programModePin, INPUT_PULLDOWN);
  
  pinMode(programModeLedPin, OUTPUT);
}


/*=================================*/
/*       Loop and Functions
/*=================================*/

/*
* Actions to be taken on the rising edge of the clock
*/
int counter = 0;
void onRisingClock() {
  a_reg->onRisingClock();
  b_reg->onRisingClock();
  mar->onRisingClock();
  ir->onRisingClock();
  alu->onRisingClock();
  ram->onRisingClock();

  counter ++;
}



/*
* The main loop
*/
void loop(){ 
  if(digitalRead(counterResetPin) == HIGH ) counter = 0;
  if(digitalRead(programModePin) == HIGH){
    ram->programMode = MAN;
    digitalWrite(programModeLedPin, HIGH);
  }
  else{
    ram->programMode = AUTO;
    digitalWrite(programModeLedPin, LOW);
  }

  #ifdef DEBUG
  debug();
  testLogic();
  #endif

  if(ram->programMode == MAN){
    ram->Program();
  }
  else if(CLK != LastClockState) {
    if(CLK == HIGH) {
      onRisingClock();

    }
    LastClockState = CLK;
  }
}

/*=================================*/
/*    Testing and Debugging
/*=================================*/

void debug() {
  Serial.print("{");
  Serial.print("\"CNT\": ");
  Serial.print(counter);
  Serial.print(", \"CLK\": ");
  Serial.print(CLK);
  Serial.print(", \"BUS\": ");
  Serial.print(bus->GetData());
  Serial.print(", \"AR\": ");
  Serial.print(a_reg->DATA);
  Serial.print(", \"BR\": ");
  Serial.print(b_reg->DATA);
  Serial.print(", \"ALU\": ");
  Serial.print(alu->result);
  Serial.print(", \"IR\": ");
  Serial.print(ir->DATA);
  Serial.print(", \"ADDR\": ");
  Serial.print(mar->DATA);
  Serial.print(", \"RAM\": ");
  Serial.print(ram->currentRam);
  Serial.println("}");
  for (int i = 0; i < 50; i++) {
    delayMicroseconds(1);
  }
}

void testLogic(){
  //load bus to A register
  if(counter == 1){
    a_reg->LATCH = 1; 
  }
  //update bus
  if(counter == 2){
    bus->SetData(0b11101101);
  }
  //load bus to B register
  if(counter == 3){
    b_reg->LATCH = 1; 
  }
  //update bus from A register
  if(counter == 4){
    a_reg->ENABLE = 1; 
  }

  //update Instruction Register from bus
  if(counter == 5){
    ir->LATCH = 1; 
  }

  if(counter == 6){
    bus->SetData(0); 
  }
  
  if(counter == 7){
    ir->ENABLE = 1; 
  }

  if(counter == 8){
    alu->sigOut = 1; 
  }

  if(counter == 9){
    alu->sub = 1; 
  }

  if(counter == 10){
    alu->sigOut = 1; 
  }

  if(counter == 11){
    ir->ENABLE = 1; 
  }

  if(counter == 12){
    mar->LATCH = 1;
  }

  if(counter == 13){
    bus->SetData(0b00000000);
  }

  if(counter == 14){
    mar->LATCH = 1;
  }

  if(counter == 15){
    bus->SetData(0b00000011);
  }

  if(counter == 16){
    ram->RamIn = 1;
  }

  if(counter == 17){
    bus->SetData(0b00000001);
  }

  if(counter == 18){
    mar->LATCH = 1;
  }

  if(counter == 19){
    bus->SetData(0b00000111);
  }

  if(counter == 20){
    ram->RamIn = 1;
  }

  if(counter == 21){
    bus->SetData(0b00000010);
  }

  if(counter == 22){
    mar->LATCH = 1;
  }

  if(counter == 23){
    bus->SetData(0b00001011);
  }

  if(counter == 24){
    ram->RamIn = 1;
  }
  
  

}