/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Utility to fade leds wired to a PWM capable pin

  Use at your own risk.
*/
#ifndef RoxFadingLed_h
#define RoxFadingLed_h
#include <Arduino.h>
#include "RoxFlags.h"

// flags
#define ROX_FADING_LED_FLAG_STATE_CHANGED  0
#define ROX_FADING_LED_FLAG_STATE          1
#define ROX_FADING_LED_FLAG_DIRECTION      2

// ***************************************
// ***************************************
//                RoxFadingLed
// ***************************************
// ***************************************
class RoxFadingLed {
private:
  int16_t pin = -1;
  unsigned long  prevTime = 0;
  RoxFlags <uint8_t> flags;
  uint8_t currentValue = 0;
  uint16_t rate = 120;
  void setLedState(bool state, bool t_changed=true){
    if(isOn() != state){
      prevTime = micros();
      flags.write(ROX_FADING_LED_FLAG_STATE, state);
      flags.write(ROX_FADING_LED_FLAG_STATE_CHANGED, t_changed);
      currentValue = 0;
      analogWrite(pin, 0);
    }
  }
public:
  RoxFadingLed(){

  }
  void begin(int16_t t_pin=-1){
    if(t_pin >= 0){
      pin = t_pin;
      pinMode(pin, OUTPUT);
      analogWrite(pin, 0);
    }
    prevTime = micros();
  }
  void setBPM(uint16_t t_value){
    rate = constrain(t_value, 30, 300);
    rate = (0x3938700 / rate) / 32;
  }
  bool isOn(){
    return flags.read(ROX_FADING_LED_FLAG_STATE);
  }
  void on(){
    setLedState(true);
  }
  void off(){
    setLedState(false);
  }
  bool toggle(){
    bool state = !isOn();
    setLedState(state);
    return state;
  }
  void update(){
    bool t = false;
    if((unsigned long) micros()-prevTime >= rate){
      t = true;
      prevTime = micros();
    }
    if(isOn() && t){
      if(flags.read(ROX_FADING_LED_FLAG_DIRECTION)){
        if(currentValue < 239){
          currentValue += 16;
        } else {
          flags.off(ROX_FADING_LED_FLAG_DIRECTION);
          currentValue = 250;
        }
      } else {
        if(currentValue >= 15){
          currentValue -= 16;
        } else {
          flags.on(ROX_FADING_LED_FLAG_DIRECTION);
          currentValue = 0;
        }
      }
      analogWrite(pin, currentValue);
    }
  }
};
#endif
