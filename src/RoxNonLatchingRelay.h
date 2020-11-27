#ifndef RoxNonLatchingRelay_h
#define RoxNonLatchingRelay_h
#include <Arduino.h>
#include "RoxFlags.h"

// flags
#define ROX_NL_RELAY_STATE 0
#define ROX_NL_RELAY_STATE_CHANGED 1
#define ROX_NL_RELAY_ACTIVE 2
#define ROX_NL_RELAY_MOMENTARY 3
#define ROX_NL_RELAY_MOMENTARY_ACTIVE 4
#define ROX_NL_RELAY_MOMENTARY_DONE 5

// ***************************************
// ***************************************
//                RoxRelay
// ***************************************
// ***************************************
template <uint16_t holdTime, bool activeState>
class RoxNonLatchingRelay {
private:
  unsigned long prevTime = 0;
  int8_t pin = -1;
  RoxFlags <uint8_t> flags;
  bool isActive(){
    return flags.read(ROX_NL_RELAY_ACTIVE);
  }
  bool stateChanged(){
    return flags.toggleIfTrue(ROX_NL_RELAY_STATE_CHANGED);
  }
  bool polarityLow(){
    return !activeState;
  }
  bool polarityHigh(){
    return activeState;
  }
public:
  RoxNonLatchingRelay(){
    if(holdTime==0){
      // holdTime can NOT be 0
      while(1);
    }
  }
  void begin(int8_t t_pin, bool defaultToSet=false){
    pin = t_pin;
    pinMode(pin, OUTPUT);
    // set pin to initial state
    digitalWrite(pin, defaultToSet ? polarityHigh() : polarityLow());
    if(defaultToSet>=0){
      flags.write(ROX_NL_RELAY_STATE, defaultToSet);

      flags.on(ROX_NL_RELAY_STATE_CHANGED);
    }
    prevTime = millis();
  }
  void momentary(bool value){
    flags.write(ROX_NL_RELAY_MOMENTARY, value);
    flags.off(ROX_NL_RELAY_MOMENTARY_ACTIVE);
  }
  bool isMomentary(){
    return flags.read(ROX_NL_RELAY_MOMENTARY);
  }
  void update(uint16_t momentaryHold=50){
    if(isMomentary() && flags.read(ROX_NL_RELAY_MOMENTARY_ACTIVE)){
      if(millis()-prevTime>=momentaryHold){
        toggle();
        flags.off(ROX_NL_RELAY_MOMENTARY_ACTIVE);
        flags.on(ROX_NL_RELAY_MOMENTARY_DONE);
      }
    }

    if(!isActive() && stateChanged()){
      digitalWrite(pin, (isSet() ? polarityHigh() : polarityLow()));
      flags.on(ROX_NL_RELAY_ACTIVE);
      prevTime = millis();
    } else if(isActive()){
      if(millis()-prevTime >= holdTime){
        flags.off(ROX_NL_RELAY_ACTIVE);
        if(!flags.toggleIfTrue(ROX_NL_RELAY_MOMENTARY_DONE) && isSet()){
          flags.on(ROX_NL_RELAY_MOMENTARY_ACTIVE);
          prevTime = millis();
        }

      }
    }
  }
  bool set(){
    if(isActive()){
      return false;
    }
    flags.on(ROX_NL_RELAY_STATE);
    flags.on(ROX_NL_RELAY_STATE_CHANGED);
    return true;
  }
  bool reset(){
    if(isActive()){
      return false;
    }
    flags.off(ROX_NL_RELAY_STATE);
    flags.on(ROX_NL_RELAY_STATE_CHANGED);
    return true;
  }
  bool isSet(){
    return flags.read(ROX_NL_RELAY_STATE);
  }
  bool toggle(){
    if(isSet()){
      reset();
    } else {
      set();
    }
    return isSet();
  }
};

#endif
