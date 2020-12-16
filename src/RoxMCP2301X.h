/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to control one or more MCP2301X multiplexers

  You can set pins as inputs and/or outpus and call .pinMode(), .digitalRead()
  or .digitalWrite() on your instance just like you would with a built-in pin

  You can use OUTPUT, INPUT or INPUT_PULLUP with .pinMode()

  Data is read from all pins of the MCP at the same time and stored in RAM

  When you write to a pin, a flag is set, next time .update() is called any
  pins that were written to will be updated in the MCP

  Initial Address 0x20

  Use at your own risk.

  GPIO on MCP23017 goes like this:
  GPA0 = pin 0
  GPA1 = pin 1
  GPA2 = pin 2
  GPA3 = pin 3
  GPA4 = pin 4
  GPA5 = pin 5
  GPA6 = pin 6
  GPA7 = pin 7
  GPB0 = pin 8
  GPB1 = pin 9
  GPB2 = pin 10
  GPB3 = pin 11
  GPB4 = pin 12
  GPB5 = pin 13
  GPB6 = pin 14
  GPB7 = pin 15
  The second MCP23017 will be
  GPA0 = pin 16
  GPA1 = pin 17
  etc...
*/
#ifndef RoxMCP2301X_h
#define RoxMCP2301X_h

#include "RoxFlags.h"
#include "RoxLed.h"
#include "Wire.h"

// pin mode
#define ROX_MCP2301X_IO_MODE 0x00
// Interrupts
#define ROX_MCP2301X_INTERRUPT_ENABLE 0x04
#define ROX_MCP2301X_INT_CONFIG 0x08
#define ROX_MCP2301X_IO_CONFIG 0x0A
// pullups
#define ROX_MCP2301X_PULLUP 0x0C
// read
#define ROX_MCP2301X_IO 0x12

#define ROX_MCP2301X_FLAG_SETUP_COMPLETE 0
#define ROX_MCP2301X_FLAG_WRITTEN_A 1
#define ROX_MCP2301X_FLAG_WRITTEN_B 2

template <uint8_t address>
class RoxMCP2301X {
private:
  uint8_t ioData[2] = {0, 0};
  uint8_t pullupData[2] = {0, 0};
  uint8_t states[2] = {0, 0};
  RoxFlags <uint8_t> flags;
  unsigned long lastTimeCheck = 0;
  void writeData(uint8_t t_reg, uint8_t t_value){
    Wire.beginTransmission(address);
    Wire.write((uint8_t) t_reg);
    Wire.write((uint8_t) t_value);
  	Wire.endTransmission();
  }
  uint8_t readData(uint8_t t_reg){
    Wire.beginTransmission(address);
    Wire.write((uint8_t) t_reg);
    Wire.endTransmission();

    Wire.requestFrom((int) address, (int) 1);
    return Wire.read();
  }
  void readPins(){
    Wire.beginTransmission(address);
    Wire.write((uint8_t) ROX_MCP2301X_IO);
  	Wire.endTransmission();
    // request 2 bytes
    // always read both ports A and B
    Wire.requestFrom((int) address, (int) 2);
    states[0] = Wire.read();
    states[1] = Wire.read();
  }
  void writePins(uint8_t index=0){
    writeData(ROX_MCP2301X_IO+index, states[0+index]);
  }
  void setAllPinsModes(){
    // input or output
    // set all to outputs by default
    writeData(ROX_MCP2301X_IO_MODE, ioData[0]);
    writeData(ROX_MCP2301X_IO, 0);
    writeData(ROX_MCP2301X_IO_MODE+1, ioData[1]);
    writeData(ROX_MCP2301X_IO+1, 0);

    // pullup resistor
    writeData(ROX_MCP2301X_PULLUP, pullupData[0]);
    writeData(ROX_MCP2301X_PULLUP+1, pullupData[1]);
  }

public:
  RoxMCP2301X(){
    flags.reset();
  }
  void begin(bool fast=false){
    Wire.begin();
    Wire.setClock((fast) ? 400000 : 100000);
    setAllPinsModes();
  }
  void pinMode(uint8_t pin, uint8_t mode){
    uint8_t index = (pin > 7) ? 1 : 0;
    pin = (pin > 7) ? pin-8 : pin;
    uint8_t inputType = 0;
    uint8_t pullup = 0;
    if(mode!=OUTPUT){
      inputType = 1;
      if(mode==INPUT_PULLUP){
        pullup = 1;
      }
    }
    bitWrite(ioData[index], pin, inputType);
    bitWrite(pullupData[index], pin, pullup);
  }
  void update(uint8_t debounceTime=1){
    if(!flags.read(ROX_MCP2301X_FLAG_SETUP_COMPLETE)){
      // in the begin method we turn all pins as outputs without the input pullup
      // resistor, after you called begin() you have to call pinMode for each pin
      // once that's all done you will call update() in your loop, however,
      // the very first time the update() is called we will update the MCP chip
      // pins to match what was done by pinMode(), this is so theres no i2c calls
      // everytime the pinMode() method is called.
      setAllPinsModes();
      flags.on(ROX_MCP2301X_FLAG_SETUP_COMPLETE);
    }
    if(flags.toggleIfTrue(ROX_MCP2301X_FLAG_WRITTEN_A)){
      // write to first 8 pins
      writePins(0);
    }
    if(flags.toggleIfTrue(ROX_MCP2301X_FLAG_WRITTEN_B)){
      // write to last 8 pins
      writePins(1);
    }
    // now we can read the MCP
    if((unsigned long)millis()-lastTimeCheck>=debounceTime){
      readPins();
      lastTimeCheck = millis();
    }
  }
  void ledControl(uint8_t pin, RoxLed &t_led, uint16_t rate=75){
    uint8_t cmd = t_led.update(rate);
    if(cmd==1){
      this->digitalWrite(pin, LOW);
    } else if(cmd==2){
      this->digitalWrite(pin, HIGH);
    }
  }
  bool digitalRead(uint8_t pin){
    uint8_t index = (pin > 7) ? 1 : 0;
    pin = (pin > 7) ? pin-8 : pin;
    return bitRead(states[index], pin);
  }
  void digitalWrite(uint8_t pin, bool value){
    uint8_t index = (pin > 7) ? 1 : 0;
    pin = (pin > 7) ? pin-8 : pin;
    // check if the pin is set to OUTPUT first then if the value is different.
    // otherwise don't do anything
    if(!bitRead(ioData[index], pin) && bitRead(states[index], pin) != value){
      bitWrite(states[index], pin, value);
      if(index==0){
        flags.on(ROX_MCP2301X_FLAG_WRITTEN_A);
      } else {
        flags.on(ROX_MCP2301X_FLAG_WRITTEN_B);
      }
    }
  }
};
template <uint8_t address> class RoxMCP23017 : public RoxMCP2301X <address> {};
template <uint8_t address> class RoxMCP23018 : public RoxMCP2301X <address> {};

#endif
