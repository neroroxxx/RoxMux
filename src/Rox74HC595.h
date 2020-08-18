/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to control one or more 74HC595 multiplexers

  This library will control each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/
#ifndef Rox74HC595_h
#define Rox74HC595_h

#include "RoxMuxUtils.h"

#define ROXMUX_74HC595_FLAG_CHANGED 0

#define ROXMUX_74HC595_DELAY 5

template <uint8_t _muxCount>
class Rox74HC595 {
private:
  uint8_t states[_muxCount];
  RoxMuxFlags <uint8_t> flags;
  int8_t clkPin     = -1;
  int8_t latchPin   = -1;
  int8_t dataPin    = -1;
  int8_t t_pwm      = -1;

public:
  Rox74HC595(){
    flags.reset();
    memset(states, 0, _muxCount);
  }
  void begin(uint8_t t_date, uint8_t t_latch, uint8_t t_clk,
    int8_t t_pwmPin = -1){
    clkPin = t_clk;
    latchPin = t_latch;
    dataPin = t_data;
    pinMode(t_clock, OUTPUT);
    pinMode(t_latch, OUTPUT);
    pinMode(t_data, OUTPUT);
    if(t_pwm!=-1){
      pinMode(t_pwm, OUTPUT);
      digitalWrite(t_pwm, HIGH);
    }
    digitalWrite(t_clock, LOW);
    digitalWrite(t_latch, HIGH);
    // turn all off
    flags.on(ROXMUX_74HC595_FLAG_CHANGED);
    update();
  }
  void update(){
    if(flags.toggleIfTrue(ROXMUX_74HC595_FLAG_CHANGED)){
      // set load pin
      digitalWrite(latchPin, LOW);
      // total number of 74HC595 chips, each chip has 8 outputs.
      // we'll always write to all the pins.
      for(uint8_t mux = 0; mux < _muxCount; mux++){
        //for(int i = 7; i >= 0; i--){
        for(uint8_t i = 0; i < 7; i++){
          uint8_t bit = bitRead(states[mux], i);
          digitalWrite(dataPin, bit);
          digitalWrite(clkPin, HIGH);
#if ROXMUX_74HC595_DELAY > 0
          delayMicroseconds(ROXMUX_74HC595_DELAY);
#endif
          digitalWrite(clkPin, LOW);
        }
      }
      digitalWrite(latchPin, HIGH);
    }
  }
  void setBrightness(uint8_t t_pwmValue){
    if(t_pwm!=-1){
      if(t_pwmValue==0 || t_pwmValue>=255){
        digitalWrite(t_pwm, ((t_pwmValue==0) ? LOW : HIGH));
      } else {
        analogWrite(t_pwm, t_pwmValue);
      }
    }
  }
  // expects a zero-index value
  void writePin(uint16_t t_pin, bool on){
    uint8_t muxIndex = (uint8_t)floor(t_pin/8.0);
    if(bitRead(states[muxIndex], t_pin) != on){
      flags.on(ROXMUX_74HC595_FLAG_CHANGED);
    }
    bitWrite(states[muxIndex], t_pin, on);
  }

};
#endif
#endif
