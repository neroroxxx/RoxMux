/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Utility to control Leds

  Use at your own risk.
*/
#ifndef RoxLed_h
#define RoxLed_h
#include <Arduino.h>
#include "RoxTimer.h"
#include "RoxFlags.h"

// flags
#define ROX_LED_FLAG_MODE_BLINK 0
#define ROX_LED_FLAG_MODE_PULSE 1
#define ROX_LED_FLAG_LED_STATE_CHANGED 2
#define ROX_LED_FLAG_LED_STATE 3
#define ROX_LED_FLAG_LED_BLINK_STATE 4
#define ROX_LED_FLAG_LED_PULSE 5

// ***************************************
// ***************************************
//                RoxLed
// ***************************************
// ***************************************
class RoxLed {
private:
  int16_t pin = -1;
  unsigned long  prevTime = 0;
  RoxFlags <uint8_t> flags;
  uint8_t mode = 0; // on off
  bool blinkMode(){
    return flags.read(ROX_LED_FLAG_MODE_BLINK);
  }
  bool pulseMode(){
    return flags.read(ROX_LED_FLAG_MODE_PULSE);
  }
  bool stateChanged(){
    return flags.toggleIfTrue(ROX_LED_FLAG_LED_STATE_CHANGED);
  }
  bool toggleBlinkState(){
    return flags.toggle(ROX_LED_FLAG_LED_BLINK_STATE);
  }
  bool getBlinkState(){
    return flags.read(ROX_LED_FLAG_LED_BLINK_STATE);
  }
  void setLedState(bool state, bool t_changed=true){
    if(isOn() != state){
      flags.write(ROX_LED_FLAG_LED_STATE, state);
      flags.write(ROX_LED_FLAG_LED_STATE_CHANGED, t_changed);
    }
  }
  bool pinControl(bool state){
    if(pin >= 0){
      digitalWrite(pin, state);
    }
    return isOn();
  }
  bool getPulseState(){
    return flags.read(ROX_LED_FLAG_LED_BLINK_STATE);
  }
public:
  RoxLed(){

  }
  void begin(int16_t t_pin=-1){
    if(t_pin >= 0){
      pin = t_pin;
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }
    prevTime = millis();
  }
  void setMode(uint8_t t_mode){
    flags.off(ROX_LED_FLAG_MODE_BLINK);
    flags.off(ROX_LED_FLAG_MODE_PULSE);
    switch(t_mode){
      case ROX_BLINK:
        flags.on(ROX_LED_FLAG_MODE_BLINK);
        break;
      case ROX_PULSE:
        flags.on(ROX_LED_FLAG_MODE_PULSE);
        break;
    }
  }
  bool isOn(){
    return flags.read(ROX_LED_FLAG_LED_STATE);
  }
  void on(){
    setLedState(true);
  }
  void off(){
    setLedState(false);
  }
  void pulse(){
    setLedState(true);
    flags.on(ROX_LED_FLAG_LED_PULSE);
  }
  bool toggle(){
    bool state = !isOn();
    setLedState(state);
    return state;
  }
  uint8_t updateBPM(uint16_t beatsPerMinute=120){
    return update(60000/beatsPerMinute);
  }
  uint8_t update(uint16_t rate=75){
    bool ret = false;
    // led is off
    if(!isOn()){
      if(stateChanged()){
        return pinControl(LOW) ? 1 : 0;
      }
      return 0;
    }
    unsigned long timeout = (unsigned long) millis() - prevTime;
    // the led is in ON state
    if(blinkMode()){
      // it's in blinking mode
      if(timeout >= rate){
        // time to toggle it
        ret = pinControl(toggleBlinkState());
        prevTime = millis();
        if(ret){
          return getBlinkState() ? 2 : 1;
        }
      }
    } else if(pulseMode()){
      if(flags.read(ROX_LED_FLAG_LED_BLINK_STATE)){
        if(timeout >= 75){
          flags.write(ROX_LED_FLAG_LED_BLINK_STATE, false);
          ret = pinControl(LOW) ? 1 : 0;
          if(flags.toggleIfTrue(ROX_LED_FLAG_LED_PULSE)){
            off();
          }
          return ret;
        }
      }
      if(timeout >= rate){
        flags.write(ROX_LED_FLAG_LED_BLINK_STATE, true);
        ret = pinControl(HIGH);
        prevTime = millis();
        return ret ? 2 : 0;
      }
    } else {
      // neither pulse nor blink mode are enabled so the led just turns on/off
      if(stateChanged()){
        pinControl(isOn());
        return isOn() ? 2 : 1;
      }
    }
    return ret;
  }
};

#endif
