#include "clock.h"

TaskHandle_t ClkLoop;
int PULSE_LED = 21;
int MODE_BTN = 12;
int PULSE_BTN = 13;
int HLT_ISR_PIN = 2;
int delayLength = 1000;
int CLK_SPEED_POT = A0;
int pulseMode = MANUAL;
bool CLK = 0; //clock output
bool halted = false;

//macros for bebounce protection on rising edge of manual pulse button 
#define DRE(signal, state) (state=(state<<1)|signal)==B00001111
uint8_t pulseButtonRisingState;

//macros for bebounce protection on falling edge of manual pulse button 
#define DFE(signal, state) (state=(state<<1)|signal)==B11110000
uint8_t pulseButtonFallingState;

/*
* Initialize the clock
* and run the task on the ESP32 core 0
*/
void init(){
  pinMode(PULSE_LED, OUTPUT); 
  xTaskCreatePinnedToCore(
                    clk_loop,   /* Task function. */
                    "clk_loop",     /* name of task. */
                    1000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &ClkLoop,      /* Task handle to keep track of created task */
                    0);      /* pin task to core 0 */
}

/*
* ISR handler for the mode button
* Toggles the pulse mode between CLOCK and MANUAL
* NOTE: it is important that the main loop is not 
* blocked for too long, otherwise the ISR will not be called
* thus, when serial debugging use multiple microsecond delays
* instead of one standard delay
*/
void modeButtonIsPressed(){
 
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
   pulseMode = pulseMode == CLOCK ? MANUAL : CLOCK;
 }
 last_interrupt_time = interrupt_time;
}

/*
* Set the Pulse state if the 
* program is not halted. 
*/
void setClockState(int state){
  if(!halted){
    CLK = state;
    digitalWrite(PULSE_LED, state); 
  }
}

void readPulseButton() {
  // Read button states every 5 ms (debounce time):
  static unsigned long lastDebounce;
  if (millis() - lastDebounce >= 5) {
    lastDebounce = millis();
    // Falling edge (if switch is pressed)
    if (DFE(digitalRead(PULSE_BTN), pulseButtonFallingState)) {
      setClockState(LOW);
    }

    // Rising edge (if switch is released)
    if (DRE(digitalRead(PULSE_BTN), pulseButtonRisingState)) {
      setClockState(HIGH);
    }
  }
}

/*
 * Perform 1 full clock cycle HIGH to LOW
 * delay between states is determined by
 * CLK_SPEED_POT (A0).
 */
void clk_loop(void * parameter){
  attachInterrupt(MODE_BTN, modeButtonIsPressed, FALLING);
  for(;;){
    if(pulseMode == CLOCK){
      delayLength = (floor(analogRead(CLK_SPEED_POT) / 10) * 10) * 2 + 1;
      setClockState(HIGH);
      delay(delayLength);
      setClockState(LOW);
      delay(delayLength);
    }
    else{
      readPulseButton();
    }
    vTaskDelay(1);
  } 
}