/*=================================*/
/*         Includes
/*=================================*/
#include "clock.h"
#include "register.h"
#include "globals.h"
#include "alu.h"
#include "ram.h"
#include "bus.h"
#include "program_counter.h"

#define DEBUG 1

/*=================================*/
/*         Global Variables        
/*=================================*/

int counterResetPin = 22;
int programModePin = 15;
int programModeLedPin = 2;


int LastClockState = 0;
Bus* bus = new Bus();
ProgramCounter* programCounter = new ProgramCounter(*bus);
AluRegister* a_reg = new AluRegister(*bus);
AluRegister* b_reg = new AluRegister(*bus);
InstructionRegister* ir = new InstructionRegister(*bus);
MemoryAddressRegister* mar = new MemoryAddressRegister(*bus);
OutputRegister* out_reg = new OutputRegister(*bus);
FlagRegister* flagRegister = new FlagRegister(*bus);
Alu* alu = new Alu(*a_reg, *b_reg, *flagRegister, *bus);
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
// int counter = 0;
void onRisingClock() {
  programCounter->onRisingClock();
  a_reg->onRisingClock();
  b_reg->onRisingClock();
  mar->onRisingClock();
  ir->onRisingClock();
  alu->onRisingClock();
  ram->onRisingClock();
  out_reg->onRisingClock();
}



/*
* The main loop
*/
void loop(){ 
  if(digitalRead(counterResetPin) == HIGH ) programCounter->Count = 0;
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
  Serial.print(programCounter->Count);
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
  Serial.print(", \"FLAG\": ");
  Serial.print(flagRegister->DATA);
  Serial.print(", \"OUT\": ");
  Serial.print(out_reg->DATA);
  Serial.println("}");
  for (int i = 0; i < 50; i++) {
    delayMicroseconds(1);
  }
}

void testLogic(){
  flagRegister->InputEnable = 1;

  //load bus to A register
  if(programCounter->Count == 1){
    a_reg->InputEnable = 1; 
  }
  //update bus
  if(programCounter->Count == 2){
    bus->SetData(0b11101101);
  }
  //load bus to B register
  if(programCounter->Count == 3){
    b_reg->InputEnable = 1; 
  }
  //update bus from A register
  if(programCounter->Count == 4){
    a_reg->OutputEnable = 1; 
  }

  //update Instruction Register from bus
  if(programCounter->Count == 5){
    ir->InputEnable = 1; 
  }

  if(programCounter->Count == 6){
    bus->SetData(0); 
  }
  
  if(programCounter->Count == 7){
    ir->OutputEnable = 1; 
  }

  if(programCounter->Count == 8){
    alu->sigOut = 1; 
  }

  if(programCounter->Count == 9){
    out_reg->InputEnable = 1;
  }

  // if(programCounter->Count == 9){
  //   alu->sub = 1; 
  // }

  // if(programCounter->Count == 10){
  //   alu->sigOut = 1; 
  // }

  // if(programCounter->Count == 11){
  //   ir->ENABLE = 1; 
  // }

  // if(programCounter->Count == 12){
  //   mar->LATCH = 1;
  // }

  // if(programCounter->Count == 13){
  //   bus->SetData(0b00000000);
  // }

  // if(programCounter->Count == 14){
  //   mar->LATCH = 1;
  // }

  // if(programCounter->Count == 15){
  //   bus->SetData(0b00000011);
  // }

  // if(programCounter->Count == 16){
  //   ram->RamIn = 1;
  // }

  // if(programCounter->Count == 17){
  //   bus->SetData(0b00000001);
  // }

  // if(programCounter->Count == 18){
  //   mar->LATCH = 1;
  // }

  // if(programCounter->Count == 19){
  //   bus->SetData(0b00000111);
  // }

  // if(programCounter->Count == 20){
  //   ram->RamIn = 1;
  // }

  // if(programCounter->Count == 21){
  //   bus->SetData(0b00000010);
  // }

  // if(programCounter->Count == 22){
  //   mar->LATCH = 1;
  // }

  // if(programCounter->Count == 23){
  //   bus->SetData(0b00001011);
  // }

  // if(programCounter->Count == 24){
  //   ram->RamIn = 1;
  // }
  
  

}