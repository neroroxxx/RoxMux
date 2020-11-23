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
    return flags.read(ROX_LED_FLAG_LED_STATE_CHANGED);
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
  void pinControl(bool state){
    if(pin >= 0){
      digitalWrite(pin, state);
    }
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
  bool toggle(){
    bool state = !isOn();
    setLedState(state);
    return state;
  }
  uint8_t update(uint8_t rate=75){
    // led is off
    if(!ledOn()){
      // check if it was recently turned off
      if(stateChanged()){
        // it was recently turned off so return 1 to notify to turn the led off
        pinControl(LOW);
        // always return a value to notify if it's being used by a mux
        return 1;
      }
      return 0;
    }
    // the led is in ON state
    if(blinkMode()){
      // it's in blinking mode
      if(millis()-prevTime >= rate){
        // time to toggle it
        pinControl(toggleBlinkState());
        prevTime = millis();
        return getBlinkState();
      }
    } else if(pulseMode()){
      // we're in pulse mode, when you turn the led on, it will go on for
      // x number of milliseconds then turn off
      // first we'll check if the state has change, meaning the led was off
      // and was recently turned on
      if(stateChanged()){
        // since the state changed and the led is now on that means we have to
        // turn the actual led on
        pinControl(HIGH);
        prevTime = millis();
        // return 2 to notify the mux that the led must be turned on
        return 2;
      } else {
        // the state may hasnt changed so now we can start timing to turn it off
        if(millis()-prevTime >= rate){
          // the time passes so let's shut the led off
          pinControl(LOW);
          // the state of the led is now off, so turn the state off and
          // also make sure that the state changed flag is off to avoid returning
          // off
          setLedState(false, false);
          // return one so the mux can turn the led off
          return 1;
        }
      }
    } else {
      // neither pulse nor blink mode are enabled so the led just turns on/off
      if(stateChanged()){
        pinControl(isOn());
        return isOn() ? 2 : 1;
      }
    }
    return 0;
  }
};

#endif
