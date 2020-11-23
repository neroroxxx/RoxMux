#ifndef RoxIntervals_h
#define RoxIntervals_h
#include <Arduino.h>
#include "RoxTimer.h"
#include "RoxFlags.h"

#define ROX_INTERVALS_FLAG_VALID 0
#define ROX_INTERVALS_FLAG_PAUSED 1
#define ROX_INTERVALS_FLAG_TOGGLE 2
#define ROX_INTERVALS_FLAG_REVERT 3

// ***************************************
// ***************************************
//                RoxIntervalsBase
// ***************************************
// ***************************************
template <typename _RoxTiming, typename intervalType_t>
class RoxIntervalsBase {
private:
  intervalType_t interval = 1000;
  intervalType_t remaining = 0;
  unsigned long  prevTime = 0;
  RoxFlags <uint8_t> flags;

  void (*onComplete)(bool toggle);

  bool timeReached(){
    return (unsigned long)(_RoxTiming::get() - prevTime) >= interval;
  }
  bool valid(){
    return flags.read(ROX_INTERVALS_FLAG_VALID);
  }
  void triggerCallback(){
    onComplete(flags.toggle(ROX_INTERVALS_FLAG_TOGGLE));
    prevTime = (unsigned long) _RoxTiming::get();
  }
public:
  RoxIntervalsBase(){
    onComplete = 0;
  }
  void begin(intervalType_t t_interval, void (*fptr)(bool toggle)){
    if(onComplete){
      // if the callback was already setup then we stop here
      return;
    }
    onComplete = fptr;
    flags.on(ROX_INTERVALS_FLAG_VALID);
    interval = t_interval;
    prevTime = (unsigned long) _RoxTiming::get();
  }
  // update the number of milliseconds that need to pass
  void update(){
    if(active() && valid()){
      if(flags.read(ROX_INTERVALS_FLAG_REVERT)){
        // the timer was paused and being restarted,
        // when you pause the timer, the timer will restart and continue
        // waiting for the amount of time that was left when paused
        if((unsigned long)(_RoxTiming::get() - (prevTime+remaining))){
          flags.off(ROX_INTERVALS_FLAG_REVERT);
          triggerCallback();
        }
        return;
      }
      if(timeReached()){
        triggerCallback();
      }
    }
  }
  bool active(){
    return !flags.read(ROX_INTERVALS_FLAG_PAUSED);
  }
  void pause(){
    if(active()){
      flags.on(ROX_INTERVALS_FLAG_PAUSED);
      remaining = (unsigned long)(_RoxTiming::get() - prevTime);
    }
  }
  void resume(){
    if(!active()){
      flags.off(ROX_INTERVALS_FLAG_PAUSED);
      flags.on(ROX_INTERVALS_FLAG_REVERT);
      prevTime = (unsigned long) _RoxTiming::get();
    }
  }
  void change(intervalType_t t_interval){
    if(t_interval > 0){
      interval = t_interval;
      prevTime = (unsigned long) _RoxTiming::get();
    }
  }
};



// uses millis() for timing
class RoxIntervals : public RoxIntervalsBase <RoxTimingMillis, uint16_t> {};
// uses micros() for timing
class RoxMicroIntervals : public RoxIntervalsBase <RoxTimingMicros, unsigned long> {};

#endif
