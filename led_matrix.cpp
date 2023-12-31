#include "led_matrix.h"


LedMatrix::LedMatrix(Bus &bus, EEPROM &eeprom, ProgramCounter &programCounter, AluRegister &a_reg, AluRegister &b_reg, InstructionRegister &ir, MemoryAddressRegister &mar, FlagRegister &flagRegister, Alu &alu, Ram &ram, uint8_t *controlStep, uint16_t *controlWord) : bus(bus), eeprom(eeprom), programCounter(programCounter), a_reg(a_reg), b_reg(b_reg), ir(ir), mar(mar), flagRegister(flagRegister), alu(alu), ram(ram){

    this->controlStep = controlStep;
    this->controlWord = controlWord;

    //The pin connected to the CS pin of the Matrix
    uint8_t CS = 5;
   
    //The delay time until the contents move left by one.
    this->delayTime = 200;

    this->lc = LedController<LedSegments,1>();

    //setup the config with the same size as the controller
    controller_configuration<LedSegments,1> conf;
    //use the specified CS pin
    conf.SPI_CS = CS;
    //set the transfer speed to the highest stable value
    conf.spiTransferSpeed = 10000000;
    //enable hardware spi
    conf.useHardwareSpi = true;

    //init the controller from the configuration
    lc.init(conf);

    //set the brightness as low as possible
    lc.setIntensity(1);

    lc.clearMatrix();
    // lc.setLed(0, 6, 0, true);
    // lc.setLed(1, 6, 0, true);

}

void LedMatrix::setRow(uint8_t segment, uint8_t row, uint8_t value, bool reverse) {
    for (uint8_t i = 0; i < 8; i++) {
        lc.setLed(segment, row, 7-i, value & (1 << i));
    }
}

void LedMatrix::UpdateDisplay(){
    // Bus Segment 0 Row 7
    this->setRow(0, 7, this->bus.GetData()); 

    // MAR Segment 0 Row 6
    this->setRow(0, 6, this->mar.DATA);
    
    // RAM Segment 0 Row 5
    this->setRow(0, 5, this->ram.currentRam);

    // IR Segment 0 Row 4
    this->setRow(0, 4, this->ir.DATA);

    // Control Step Segment 0 Row 3
    this->setRow(0, 3, *this->controlStep);

    // lower 8 bits of control word Segment 1 Row 0
    this->setRow(1, 0, *this->controlWord & 0xFF);

    // upper 8 bits of control word Segment 0 Row 0
    this->setRow(0, 0, (*this->controlWord >> 8) & 0xFF);




    // PC Segment 1 Row 7
    this->setRow(1, 7, this->programCounter.Count);

    // Flag Segment 1 Row 6
    this->setRow(1, 6, this->flagRegister.DATA);

    // A Segment 1 Row 5
    this->setRow(1, 5, this->a_reg.DATA);

    // ALU Segment 1 Row 3
    this->setRow(1, 4, this->alu.result);

    // B Segment 1 Row 4
    this->setRow(1, 3, this->b_reg.DATA);


}