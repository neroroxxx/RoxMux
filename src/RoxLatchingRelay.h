#ifndef RoxLatchingRelay_h
#define RoxLatchingRelay_h
#include <Arduino.h>
#include "RoxFlags.h"

// flags
#define ROX_L_RELAY_STATE 0
#define ROX_L_RELAY_STATE_CHANGED 1
#define ROX_L_RELAY_ACTIVE 2
#define ROX_L_RELAY_MOMENTARY 3
#define ROX_L_RELAY_MOMENTARY_ACTIVE 4
#define ROX_L_RELAY_MOMENTARY_DONE 5


// ***************************************
// ***************************************
//                RoxRelay
// ***************************************
// ***************************************
template <uint16_t holdTime, bool activeState>
class RoxLatchingRelay {
private:
  unsigned long prevTime = 0;
  int8_t pin[2];
  RoxFlags <uint8_t> flags;
  bool isActive(){
    return flags.read(ROX_L_RELAY_ACTIVE);
  }
  bool stateChanged(){
    return flags.toggleIfTrue(ROX_L_RELAY_STATE_CHANGED);
  }
  bool polarityLow(){
    return !activeState;
  }
  bool polarityHigh(){
    return activeState;
  }
public:
  RoxLatchingRelay(){
    if(holdTime==0){
      // holdTime can NOT be 0
      while(1);
    }
  }
  void begin(int8_t t_reset, int8_t t_set, int8_t defaultToSet=-1){
    pin[0] = t_reset;
    pin[1] = t_set;
    pinMode(pin[ROX_RESET], OUTPUT);
    pinMode(pin[ROX_SET], OUTPUT);
    digitalWrite(pin[ROX_RESET], polarityLow());
    digitalWrite(pin[ROX_SET], polarityLow());
    if(defaultToSet>=0){
      flags.write(ROX_L_RELAY_STATE, defaultToSet);
      flags.on(ROX_L_RELAY_STATE_CHANGED);
    }
    prevTime = millis();
  }
  void momentary(bool value){
    flags.write(ROX_L_RELAY_MOMENTARY, value);
    flags.off(ROX_L_RELAY_MOMENTARY_ACTIVE);
  }
  bool isMomentary(){
    return flags.read(ROX_L_RELAY_MOMENTARY);
  }
  void update(uint16_t momentaryHold=50){
    if(isMomentary() && flags.read(ROX_L_RELAY_MOMENTARY_ACTIVE)){
      if(millis()-prevTime>=momentaryHold){
        toggle();
        flags.off(ROX_L_RELAY_MOMENTARY_ACTIVE);
        flags.on(ROX_L_RELAY_MOMENTARY_DONE);
      }
    }
    if(!isActive() && stateChanged()){
      digitalWrite(pin[isSet() ? ROX_SET : ROX_RESET], polarityHigh());
      flags.on(ROX_L_RELAY_ACTIVE);
      prevTime = millis();
    } else if(isActive()){
      if(millis()-prevTime >= holdTime){
        digitalWrite(pin[isSet() ? ROX_SET : ROX_RESET], polarityLow());
        flags.off(ROX_L_RELAY_ACTIVE);
        if(!flags.toggleIfTrue(ROX_L_RELAY_MOMENTARY_DONE)){
          flags.on(ROX_L_RELAY_MOMENTARY_ACTIVE);
          prevTime = millis();
        }

      }
    }
  }
  void set(){
    if(isActive()){
      return;
    }
    flags.on(ROX_L_RELAY_STATE);
    flags.on(ROX_L_RELAY_STATE_CHANGED);
  }
  void reset(){
    if(isActive()){
      return;
    }
    flags.off(ROX_L_RELAY_STATE);
    flags.on(ROX_L_RELAY_STATE_CHANGED);
  }
  bool isSet(){
    return flags.read(ROX_L_RELAY_STATE);
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
