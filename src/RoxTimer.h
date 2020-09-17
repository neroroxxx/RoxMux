#ifndef RoxTimer_h
#define RoxTimer_h
#include <Arduino.h>
// classes used to get the time in milliseconds or microseconds
class RoxTimingMillis {
  public: static unsigned long get(){ return (unsigned long) millis(); }
};
class RoxTimingMicros {
  public: static unsigned long get(){ return (unsigned long) micros(); }
};

// ***************************************
// ***************************************
//                RoxTimerBase
// base class for timers
// ***************************************
// ***************************************
template <typename _RoxTiming, typename intervalType_t>
class RoxTimerBase {
private:
  intervalType_t interval = 1000;
  unsigned long  prevTime = 0;
public:
  RoxTimerBase(){
    stop();
  }
  operator bool(){
    return complete();
  }
  // start the timer, pass number of milliseconds as the parameter
  void start(intervalType_t value){
    interval = value;
    prevTime = (unsigned long) _RoxTiming::get();
  }
  // update the number of milliseconds that need to pass
  void update(intervalType_t value){
    if(active()){
      interval = value;
    }
  }
  // trigger the timer as complete,
  // this will make the complete() method return true the next time
  // useful if something you were waiting for happened already
  // elsewhere in your code
  void trigger(){
    start(0);
  }
  // returns true if the timer is currently running
  bool active(){
    return (prevTime>0);
  }
  // returns true if the time specified has been reached
  // after that point the complete() method will return false until
  // start() has been called again
  bool complete(){
    if(active() && (unsigned long)(_RoxTiming::get() - prevTime) >= interval) {
      stop();
      return true;
    }
    return false;
  }
  bool reached(){ return complete(); }
  bool expired(){ return complete(); }
  // return the number of milliseconds remaining before complete is triggered
  unsigned long remaining(){
    if(active()){
      return (unsigned long)(_RoxTiming::get() - prevTime);
    }
    return 0;
  }
  // stop the timer
  void stop(){
    prevTime = 0;
  }
};

// ***************************************
// ***************************************
//          RoxEndlessTimerBase
// ***************************************
// ***************************************
template <typename _RoxTiming, typename intervalType_t>
class RoxEndlessTimerBase {
private:
  RoxTimerBase <_RoxTiming, intervalType_t> timer;
  intervalType_t interval = 1000;
public:
  RoxEndlessTimerBase(){}
  operator bool(){
    return complete();
  }
  void start(intervalType_t value){
    interval = value;
    timer.start(interval);
  }
  bool reached(){ return complete(); }
  bool expired(){ return complete(); }
  bool complete(){
    if(timer.complete()){
      timer.start(interval);
      return true;
    }
    return false;
  }
  void stop(){
    end();
  }
  void end(){
    timer.stop();
  }
};


// uses millis() for timing
class RoxTimer : public RoxTimerBase <RoxTimingMillis, uint16_t> {};
// uses micros() for timing
class RoxMicroTimer : public RoxTimerBase <RoxTimingMicros, unsigned long> {};

// uses millis() for timing
class RoxEndlessTimer : public RoxEndlessTimerBase <RoxTimingMillis, uint16_t> {};
// uses micros() for timing
class RoxMicroEndless : public RoxEndlessTimerBase <RoxTimingMicros, unsigned long> {};


#endif
