/*
  https://www.roxxxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to read one or more 74HC165 multiplexers

  This library will read each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/
#ifndef Rox74HC165_h
#define Rox74HC165_h

#include <Arduino.h>

#define ROXMUX_74HC165_DELAY 1

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
      memset(states, 0, sizeof(states[0])*_muxCount);
    }
    uint16_t getLength(){
      return _muxCount*8;
    }
    void update(){
      digitalWrite(loadPin, LOW);
      delayMicroseconds(ROXMUX_74HC165_DELAY);
      digitalWrite(loadPin, HIGH);
      for(uint8_t mux = 0; mux < _muxCount; mux++){
        for(int i = 7; i >= 0; i--){
          uint8_t bit = digitalRead(dataPin);
          bitWrite(states[mux], i, bit);
          digitalWrite(clkPin, HIGH);

          delayMicroseconds(ROXMUX_74HC165_DELAY);

          digitalWrite(clkPin, LOW);
        }
      }
    }
    // return FALSE/LOW if bit is set (pin is grounded)
    bool read(uint16_t n){
      if(n >= (_muxCount*8)){
        return HIGH;
      }
      return bitRead(states[(n>>3)], (n&0x07));
    }
    bool readPin(uint16_t n){
      return read(n);
    }
};

template <uint8_t _muxinCount>
class RoxMUXIN16 : public Rox74HC165 <_muxinCount*2>{};


#endif
