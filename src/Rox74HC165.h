/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to read one or more 74HC165 multiplexers

  This library will read each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/

#ifndef Rox74HC165_h
#define Rox74HC165_h

#include <Arduino.h>

template <uint8_t _muxCount>
class Rox74HC165 {
  private:
    uint8_t clkPin  = 0;
    uint8_t loadPin = 0;
    uint8_t dataPin = 0;
    uint8_t states[_muxCount];
  public:
    Rox74HC165(){}
    void begin(uint8_t t_data, uint8_t t_load, uint8_t t_clk){
      if(t_clk==t_load || t_clk==t_data || t_load==t_data){
        Serial.println("invalid 74HC165 pins used");
        while(1);
      }
      clkPin = t_clk;
      loadPin = t_load;
      dataPin = t_data;
      pinMode(clkPin, OUTPUT);
      pinMode(loadPin, OUTPUT);
      pinMode(dataPin, INPUT);
      digitalWrite(clkPin, LOW);
      digitalWrite(loadPin, HIGH);
    }
    void update(){
      memset(states, 0, _muxCount);
      digitalWrite(loadPin, LOW);
      delayMicroseconds(5);
      digitalWrite(loadPin, HIGH);
      for(uint8_t mux = 0; mux < _muxCount; mux++){
        for(int i = 7; i >= 0; i--){
          uint8_t bit = digitalRead(dataPin);
          bitWrite(states[mux], i, bit);
          digitalWrite(clkPin, HIGH);
          delayMicroseconds(5);
          digitalWrite(clkPin, LOW);
        }
      }
    }
    uint8_t read(uint8_t n){
      n = constrain(n, 0, _muxCount-1);
      return states[n];
    }
    // read the first 4 mux starting from the first one
    // if there are less than 4 mux then just returns all of them
    uint32_t read(){
      return read(0, _muxCount-1);
    }
    // return the values of up to 4 muxes at a time
    uint32_t read(uint8_t first, uint8_t last){
      if(_muxCount==1){
        return states[0];
      }
      if(last>=_muxCount || first>=_muxCount || first>=last){
        return 0;
      }
      uint32_t x = 0;
      for(uint8_t i = 0, e=first; i < 4; i++, e++){
        if(e>=_muxCount){
          break;
        }
        x |= states[e] << (i*8);
      }
      return x;
    }
    bool readPin(uint16_t n){
      return readPin((n>>3), (n&0x07));
    }
    // return true if the pin is active
    bool readPin(uint8_t t_mux, uint8_t t_bit){
      t_mux = constrain(t_mux, 0, (_muxCount-1));
      t_bit = constrain(t_bit, 0, 7);
      return !bitRead(states[t_mux], t_bit);
    }
    // return all the pins of the specified mux
    uint8_t readPins(uint8_t t_mux){
      t_mux = constrain(t_mux, 0, (_muxCount-1));
      return states[t_mux];
    }
};

template <uint8_t _muxCount>
class RoxDual74HC165 {
private:
  Rox74HC165 <(_muxCount*2)> mux;
public:
  RoxDual74HC165(){}
  void begin(uint8_t t_data, uint8_t t_load, uint8_t t_clk){
    mux.begin(t_data, t_load, t_clk);
  }
  void update(){
    mux.update();
  }
  // return true if the pin is active
  bool readPin(uint8_t t_mux=0, uint8_t t_bit=0){
    t_mux = constrain(t_mux, 0, (_muxCount-1));
    t_bit = constrain(t_bit, 0, 15);
    if(t_bit>=8){
      t_mux++;
      t_bit -= 8;
    }
    if(t_mux>=(_muxCount*2)){
      return false;
    }
    return mux.readPin(t_mux, t_bit);
  }
  uint16_t readPins(uint16_t t_mux){
    if((t_mux+1)>=(_muxCount*2)){
      return 0;
    }
    return mux.readPins(t_mux) | (mux.readPins(t_mux+1)<<8);
  }
};

#endif
