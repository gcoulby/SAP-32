#include "controller.h"
#include <Adafruit_MCP23X17.h>
#include <Wire.h>

/*
* All of the business logic is handled 
* by the controller class
*/
TwoWire i2c = TwoWire(0);
Adafruit_MCP23X17 mcp;

// delay(2000);
Controller* controller = new Controller(mcp);


void setup() {
  Serial.begin(500000);

  i2c.begin(21,22,100000); // SDA pin 21, SCL pin 22, 100kHz frequency
    Serial.println("MCP23017 Test!");
    // Serial.println(mcp.begin_I2C(0x27,&i2c ));
    if (!mcp.begin_I2C(0x27,&i2c )) {
    //if (!mcp.begin_SPI(CS_PIN)) {
      Serial.println("Error.");
      while (1);
    }
    delay(2000);

    for(int i = 0; i < 12; i++){
        mcp.pinMode(i, INPUT_PULLUP);
    }


    // for (int i = 0; i < 8; i++)
    // {
    //     int a = mcp.digitalRead(i); 
    //     Serial.println("data channel " + String(i) + " = " + String(a));
    // }
}

void loop(){ 
  controller->handleEmulationCycle();
}
