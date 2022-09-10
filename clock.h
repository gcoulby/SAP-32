#ifndef CLOCK_H
#define CLOCK_H
#include <Arduino.h>

extern bool CLK;
extern bool halted;

enum PulseMode { CLOCK, MANUAL };

void init();
void setClockState(int state);
void clk_loop(void * parameter);
void modeButtonIsPressed();

#endif
