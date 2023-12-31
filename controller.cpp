#include "controller.h"


int counterResetPin = 4; 
int programModePin = 33; 
int programModeLedPin = 13; 

// #define DEBUG 1
bool CLK = 0;
bool HALT_CLOCK = false;
int CLK_pin = 2; 
int CLK_HLT_pin = 27;
int DEBUG_pin = 14;




/*=================================*/
/*       Setup                  
/*=================================*/

Controller::Controller(Adafruit_MCP23X17 &mcp) : mcp(mcp) {

    this->eeprom = new EEPROM();
    this->bus = new Bus();
    this->mar = new MemoryAddressRegister(*bus);
    this->ram = new Ram(*bus, *mar, mcp);
    this->programCounter = new ProgramCounter(*bus);
    this->a_reg = new AluRegister(*bus);
    this->b_reg = new AluRegister(*bus);
    this->ir = new InstructionRegister(*bus);
    this->out_reg = new OutputRegister(*bus);
    this->flagRegister = new FlagRegister(*bus,*a_reg, *b_reg);
    this->alu = new Alu(*a_reg, *b_reg, *flagRegister, *bus);
    
    //===== Initial states =====//
    this->ControlWord = 0;
    this->LastClockState = 0;
    this->controlStep = 0;
    this->maxControlStepCount = 4;
    this->lastRamValue = 0;
    this->lastAddress = 0;
    
    this->ledMatrix = new LedMatrix(*bus, *eeprom, *programCounter, *a_reg, *b_reg, *ir, *mar, *flagRegister, *alu, *ram, &controlStep, &ControlWord);   
    pinMode(counterResetPin, INPUT_PULLDOWN);
    pinMode(DEBUG_pin, INPUT_PULLDOWN);
    pinMode(programModePin, INPUT_PULLDOWN);
    pinMode(programModeLedPin, OUTPUT);
    pinMode(CLK_HLT_pin, OUTPUT);
    this->onRisingClock(true);
}


/*=================================*/
/*       Loop and Functions
/*=================================*/
/*
* This is called by the Ardiuno loop function
* and is execute every clock cycle of the ESP32
*/
int RSTCounter = 0;

void Controller::handleEmulationCycle(){

    digitalWrite(CLK_HLT_pin, HALT_CLOCK);
    // if(HALT_CLOCK){
    // }
//   //initialize the program counter
  if(digitalRead(counterResetPin) == HIGH) {
    this->Reset();
    RSTCounter++;
    if(RSTCounter == 100){
        RSTCounter = 0;
    }
  }
  
  //Check the I/O to see if program mode should be auto or manual
  if(digitalRead(programModePin) == HIGH){
    ram->programMode = MAN;
    digitalWrite(programModeLedPin, HIGH);
  }
  else{
    ram->programMode = AUTO;
    digitalWrite(programModeLedPin, LOW);
  }


    CLK = digitalRead(CLK_pin);
    int d = digitalRead(DEBUG_pin);
    if(d == HIGH){
        debug();
    }

  #ifdef DEBUG
  //output debug logic
  debug();
//   testLogic();
  #endif


  if(ram->programMode == MAN){
    ram->Program();

    if(ram->currentRam != this->lastRamValue || mar->DATA != this->lastAddress){
        this->ledMatrix->UpdateDisplay();
        this->lastRamValue = ram->currentRam;
        this->lastAddress = mar->DATA;

        Serial.println("Last Ram Value: " + String(this->lastRamValue) + " Last Address: " + String(this->lastAddress) + " Current Ram Value: " + String(ram->currentRam) + " Current Address: " + String(mar->DATA));
        // delay(300);
    }
  }
  else if(CLK != LastClockState) {
    if(CLK == HIGH) {
      this->onRisingClock();
    }
    LastClockState = CLK;
  }
}

void Controller::incrementControlStep(){
    controlStep++;
    if(controlStep > maxControlStepCount) controlStep = 0;
}


// void Controller::handleInstruction(uint16_t instruction){
//     if(instruction & HC){ 
//         HALT_CLOCK = true; // Halt Clock
//     }
//     if(instruction & MI){
//         this->mar->InputEnable = true; // Memory address register in
//     }
//     if(instruction & RI){
//         this->ram->RamIn = true; // Ram data in
//     }
//     if(instruction & RO){
//         this->ram->RamOut = true; // Ram data out
//     }
//     if(instruction & IO){
//         this->ir->OutputEnable = true; // Instruction register out
//     }
//     if(instruction & II){
//         this->ir->InputEnable = true; // Instruction register in
//     }
//     if(instruction & AI){
//         this->a_reg->InputEnable = true; // A register in
//         this->alu->sum();
//         this->flagRegister->setFlags();
//     }
//     if(instruction & AO){
//         this->a_reg->OutputEnable = true; // A register out
//     }
//     if(instruction & EO){
//         if(instruction & SU){
//             this->alu->sub = true; // ALU subtract
//         }
//         if(instruction & FI){
//             this->flagRegister->InputEnable = true;
//         }
//         // this->alu->sum();
//         // this->flagRegister->setFlags();
//         this->alu->sigOut = true; // ALU out



//     }
//     // if(instruction & SU){
//     //     this->alu->sub = true; // ALU subtract
//     // }
//     if(instruction & BI){
//         this->b_reg->InputEnable = true; // B register in
//         this->alu->sum();
//         this->flagRegister->setFlags();
//     }
//     if(instruction & OI){
//         this->out_reg->InputEnable = true; // Output register in
//     }
//     if(instruction & CE){
//         this->programCounter->CounterEnable = true; // Program counter enable
//     }
//     if(instruction & CO){
//         this->programCounter->CounterOut = true; // Program counter out
//     }
//     if(instruction & J){
//         this->programCounter->Jump = true; // Jump (Program counter in)
//     }
//     // if(instruction & FI){
//     //     this->flagRegister->InputEnable = true; // Flags in
//     // }   
// }




void Controller::handleInstruction(uint16_t instruction){
    if(instruction & HC){ 
        HALT_CLOCK = true; // Halt Clock
    }
    if(instruction & CO){
        this->programCounter->CounterOut = true; // Program counter out
        this->programCounter->handleOutput();
    }
    if(instruction & EO){
        int8_t co = 0;
        if(instruction & SU){
            this->bus->SetData(this->a_reg->DATA - this->b_reg->DATA);
            if((this->a_reg->DATA - this->b_reg->DATA) > 255 || this->a_reg->DATA > 0){
                co = 1;
            }
            // CHECK THE CARRYS in different programs
        }
        else{
            this->bus->SetData(this->a_reg->DATA + this->b_reg->DATA);
            if((this->a_reg->DATA + this->b_reg->DATA) > 255){
                co = 1;
            }
        }
        if(instruction & FI){
            this->flagRegister->DATA = co << 1 | (this->bus->GetData()?0:2);
        }
        
        
        // if(instruction & SU){
        //     this->alu->sub = true; // ALU subtract
        // }
        // this->alu->sum();
        // this->flagRegister->setFlags();
        // if(instruction & FI){
        //     this->flagRegister->InputEnable = true;
        // }
        // this->alu->sigOut = true; // ALU out
        // this->alu->handleOutput();
    }
    if(instruction & AO){
        this->a_reg->OutputEnable = true; // A register out
        this->a_reg->handleOutput();
    }
    if(instruction & IO){
        this->ir->OutputEnable = true; // Instruction register out
        this->ir->handleOutput();
    }
    if(instruction & RO){
        this->ram->RamOut = true; // Ram data out
        this->ram->handleOutput();
    }
    if(instruction & J){
        this->programCounter->Jump();// = true; // Jump (Program counter in)
    }
    if(instruction & CE){
        this->programCounter->CounterEnable = true; // Program counter enable
        this->programCounter->handleInput();
    }
    if(instruction & MI){
        this->mar->InputEnable = true; // Memory address register in
        this->mar->handleInput();
    }
    if(instruction & RI){
        this->ram->RamIn = true; // Ram data in
        this->ram->handleInput();
    }
    if(instruction & II){
        this->ir->InputEnable = true; // Instruction register in
        this->ir->handleInput();
    }
    if(instruction & AI){
        this->a_reg->InputEnable = true; // A register in
        this->a_reg->handleInput();
        this->alu->sum();
        this->flagRegister->setFlags();
    }
    if(instruction & BI){
        this->b_reg->InputEnable = true; // B register in
        this->b_reg->handleInput();
        this->alu->sum();
        this->flagRegister->setFlags();
    }
    if(instruction & OI){
        this->out_reg->InputEnable = true; // Output register in
        this->out_reg->handleInput();
    }
}







// void Controller::handleOutputs(){
//     // this->flagRegister->setFlags();
//     this->a_reg->handleOutput();
//     this->b_reg->handleOutput();
//     this->ir->handleOutput();
//     this->alu->handleOutput();
//     this->ram->handleOutput();
//     this->programCounter->handleOutput();
// }

// void Controller::handleInputs(){
//     this->programCounter->handleInput();
//     this->mar->handleInput();
//     this->ram->handleInput();
//     this->ir->handleInput();
//     this->a_reg->handleInput();
//     this->b_reg->handleInput();
//     this->out_reg->handleInput();
// }

void Controller::Reset(){
    this->a_reg->CLR();
    this->b_reg->CLR();
    this->ir->CLR();
    this->mar->CLR();
    this->out_reg->CLR();
    this->flagRegister->CLR();
    this->alu->result = 0;
    this->controlStep = 0;
    this->ControlWord = 0;
    this->programCounter->Count = 0;
    this->InitialiseCtrlBits();
    HALT_CLOCK = false;
    this->onRisingClock(true);
}

/*
* This is executed by emulationCycle
* everytime the EMULATED CLOCK goes high
*/
void Controller::onRisingClock(bool skipIncrement){
    //increment the control step if not skipping (done on reset)
    if(HALT_CLOCK) return;
    if(!skipIncrement) this->incrementControlStep();

    this->InitialiseCtrlBits();
    // this->flagRegister->setFlags();
    uint16_t instruction = 0;

    if(controlStep < 2){
        instruction = eeprom->fetchMCodeInstruction(0, controlStep);
        this->ControlWord = instruction;
    }
    else{
        uint8_t instructionCode = this->ir->fetchInstructionCode();
        switch(instructionCode){
            case JC:
                instruction = eeprom->fetchMCodeInstruction(flagRegister->carrySet() ? instructionCode : 0, controlStep);
                break;
            case JZ:
                instruction = eeprom->fetchMCodeInstruction(flagRegister->zeroSet() ? instructionCode : 0, controlStep);
                break;
            default:
                instruction = eeprom->fetchMCodeInstruction(instructionCode, controlStep);
                break;
        }
        this->ControlWord = instruction;
    }
    this->handleInstruction(instruction);

    //First handleOutputs
    
    // this->handleOutputs();
    // this->handleInputs();
    // this->alu->sum();
    // this->flagRegister->setFlags();
 
    

      //update display each emu cycle
    this->out_reg->UpdateDisplay();
    this->ledMatrix->UpdateDisplay();
}

void Controller::InitialiseCtrlBits(){
    this->programCounter->InitialiseCtrlBits();
    this->mar->InitialiseCtrlBits();
    this->ram->InitialiseCtrlBits();
    this->ir->InitialiseCtrlBits();
    this->a_reg->InitialiseCtrlBits();
    this->b_reg->InitialiseCtrlBits();
    this->alu->InitialiseCtrlBits();
    this->out_reg->InitialiseCtrlBits();
    this->bus->InitialiseCtrlBits();
}



/*=================================*/
/*    Testing and Debugging
/*=================================*/
void Controller::debug() {
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
  Serial.print(", \"CTRL\": ");
  Serial.print(this->ControlWord);
  Serial.print(", \"C_STP\": ");
  Serial.print(this->controlStep);
  Serial.print(", \"CARRY\": ");
  Serial.print(CARRY);
  Serial.print(", \"ZERO\": ");
  Serial.print(ZERO);
  Serial.println("}");
  for (int i = 0; i < 50; i++) {
    delayMicroseconds(1);
  }
}
