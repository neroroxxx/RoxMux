/*
  
  This is an example of RoxEndlessTimer, a simple non-blocking (no delay) timer.
  This example will print a message every 2 seconds to the Serial Monitor.
  RoxEndlessTimer will continuously run at whatever interval set by the .start() method
  you may change the interval at any time by calling .start() with a different time
  If you need a one-time timer, see the RoxTimer example
*/
#include <RoxMux.h>

// initialize the timer
RoxEndlessTimer timer;

// if you want to pass the interval time in microseconds use RoxMicroEndless
// RoxMicroEndless timer;

void setup(){
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxEndlessTimer Example");
  
  // start the timer, the only argument is the time in milliseconds
  // if using RoxMicroEndless the time will be in microseconds
  timer.start(2000);
}

void loop(){
  // check if the timer has reach it's threshold
  if(timer.complete()){
    Serial.println("Timer Complete");
    // the .complete() method will be true every 2 seconds.
    // you can change the interval at any time by calling the .start() method
    // keep in mind that the .start() method will reset the timer and start over.
  }
  if(timer.active()){
    // you can also check if the timer is active at any point from another part of your code
  }
  // use timer.stop() to stop the timer completely
}
