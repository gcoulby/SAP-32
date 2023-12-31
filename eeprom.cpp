#include "eeprom.h"

EEPROM::EEPROM() {}

uint16_t EEPROM::fetchMCodeInstruction(int instruction, int step){
    return MCode[instruction][step];
}
