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

#include "RoxFlags.h"
#include "RoxLed.h"

#define ROXMUX_74HC595_FLAG_CHANGED 0
#define ROXMUX_74HC595_FLAG_BLINK_ENABLED 1
#define ROXMUX_74HC595_FLAG_BLINK_STATE 2

#define ROXMUX_74HC595_DELAY 1

template <uint8_t _muxCount, uint16_t blinkSpeed=50>
class Rox74HC595 {
private:
  uint8_t states[_muxCount];
  uint8_t blinkStates[_muxCount];
  unsigned long blinkTimer;
  RoxFlags <uint8_t> flags;
  int8_t clkPin     = -1;
  int8_t latchPin   = -1;
  int8_t dataPin    = -1;
  int8_t pwmPin     = -1;

  void _updateMuxReverse(){
    for(int mux = _muxCount-1; mux >= 0; mux--){
      for(uint8_t mask=0x80; mask; mask >>= 1){
        _writeToMux(mux, mask);
      }
    }
  }
  void _updateMux(){
    for(int mux = 0; mux < _muxCount; mux++){
      for(uint8_t mask=0x80; mask; mask >>= 1){
        _writeToMux(mux, mask);
      }
    }
  }
  void _writeToMux(uint8_t mux, uint8_t mask){
    if(flags.read(ROXMUX_74HC595_FLAG_BLINK_ENABLED)){
      if((states[mux] & mask) && (blinkStates[mux] & mask)){
        digitalWrite(dataPin, flags.read(ROXMUX_74HC595_FLAG_BLINK_STATE));
      } else {
        digitalWrite(dataPin, (states[mux] & mask));
      }
    } else {
      digitalWrite(dataPin, (states[mux] & mask));
    }

    digitalWrite(clkPin, HIGH);
#if ROXMUX_74HC595_DELAY > 0
    delayMicroseconds(ROXMUX_74HC595_DELAY);
#endif
    digitalWrite(clkPin, LOW);
    digitalWrite(dataPin, LOW);
  }

public:
  Rox74HC595(){
    flags.reset();
    memset(states, 0, sizeof(states[0])*_muxCount);
  }
  void begin(uint8_t t_data, uint8_t t_latch, uint8_t t_clk,
    int8_t t_pwm = -1){
    clkPin = t_clk;
    latchPin = t_latch;
    dataPin = t_data;
    pinMode(clkPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    if(t_pwm > -1){
      pwmPin = t_pwm;
      pinMode(pwmPin, OUTPUT);
      delay(1);
      setBrightness(255);
    }
    digitalWrite(clkPin, LOW);
    digitalWrite(latchPin, HIGH);
    // turn all off
    flags.on(ROXMUX_74HC595_FLAG_CHANGED);
    update();
  }
  void update(){
    if(flags.toggleIfTrue(ROXMUX_74HC595_FLAG_CHANGED)){
      // set load pin
      digitalWrite(latchPin, LOW);
      digitalWrite(dataPin, LOW);
      if(_muxCount>1){
        _updateMuxReverse();
      } else {
        _updateMux();
      }
      digitalWrite(latchPin, HIGH);
    }
    if((millis()-blinkTimer)>=blinkSpeed){
      if(flags.read(ROXMUX_74HC595_FLAG_BLINK_ENABLED)){
        flags.on(ROXMUX_74HC595_FLAG_CHANGED);
        flags.toggle(ROXMUX_74HC595_FLAG_BLINK_STATE);
      }
      blinkTimer = millis();
    }
  }
  void setBrightness(uint8_t value){
    if(pwmPin > -1){
      // reverse the brightness as the 595 requires a HIGH to disable outputs
      value = map(value, 0, 255, 255, 0);
#if defined(ESP32)
      digitalWrite(pwmPin, value>0);
#else
      analogWrite(pwmPin, value);
#endif
    }
  }
  void allOff(){
    for(uint8_t i=0;i<_muxCount;i++){
      if(states[i]>0){
        flags.on(ROXMUX_74HC595_FLAG_CHANGED);
        break;
      }
    }
    memset(states, 0, _muxCount);
  }
  void allOn(){
    for(uint8_t i=0;i<_muxCount;i++){
      if(states[i]==0){
        flags.on(ROXMUX_74HC595_FLAG_CHANGED);
        break;
      }
    }
    memset(states, 255, _muxCount);
  }
  void ledControl(uint8_t pin, RoxLed &t_led, uint16_t rate=75){
    uint8_t cmd = t_led.update(rate);
    if(cmd==1){
      writePin(pin, LOW);
    } else if(cmd==2){
      writePin(pin, HIGH);
    }
  }
  // expects a zero-index value
  void writePin(uint16_t t_pin, bool on){
    uint8_t muxIndex = (uint8_t)floor(t_pin/8.0);
    if(muxIndex>0){
      t_pin -= (muxIndex*8);
    }
    if(bitRead(states[muxIndex], t_pin) != on){
      flags.on(ROXMUX_74HC595_FLAG_CHANGED);
    }
    bitWrite(states[muxIndex], t_pin, on);
  }
  bool readPin(uint16_t t_pin){
    uint8_t muxIndex = (uint8_t)floor(t_pin/8.0);
    if(muxIndex>0){
      t_pin -= (muxIndex*8);
    }
    return bitRead(states[muxIndex], t_pin);
  }
  void togglePin(uint16_t t_pin){
    uint8_t muxIndex = (uint8_t)floor(t_pin/8.0);
    if(muxIndex>0){
      t_pin -= (muxIndex*8);
    }
    bool state = bitRead(states[muxIndex], t_pin);
    bitWrite(states[muxIndex], t_pin, !state);
    flags.on(ROXMUX_74HC595_FLAG_CHANGED);
  }
  void blinkPin(uint16_t t_pin, bool on){
    uint8_t muxIndex = (uint8_t)floor(t_pin/8.0);
    if(muxIndex>0){
      t_pin -= (muxIndex*8);
    }
    if(bitRead(blinkStates[muxIndex], t_pin) != on){
      flags.on(ROXMUX_74HC595_FLAG_CHANGED);
      bitWrite(blinkStates[muxIndex], t_pin, on);
    }
    // turn the flag on then check if any other blink states are on
    flags.off(ROXMUX_74HC595_FLAG_BLINK_ENABLED);
    for(uint8_t i=0;i<_muxCount;i++){
      if(blinkStates[i]>0){
        flags.on(ROXMUX_74HC595_FLAG_BLINK_ENABLED);
        break;
      }
    }

  }
};
#endif
