/*
  
  This is an example of RoxTimer, a simple non-blocking (no delay) timer.
  This example will print a message every 2 seconds to the Serial Monitor.
  RoxTimer is a one-time timer, you can restart it after it reaches it's threshold.
  If you need a timer that constantly runs, see the RoxTimerEndless example

*/
#include <RoxMux.h>

// Initialize the timer
RoxTimer timer;

// if you want to pass the interval time in microseconds use RoxMicroTimer
// RoxMicroTimer timer;

void setup(){
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxTimer Example");
  
  // start the timer, the only argument is the time in milliseconds
  // if using RoxMicroTimer the time will be in microseconds
  timer.start(2000);
}

void loop(){
  // check if the timer has reach it's threshold
  if(timer.complete()){
    Serial.println("Timer Complete");
    // restart the timer
    // RoxTimer is a one-time timer, you must restart after calling the .complete() method
    // calling the .complete() method will reset the timer so it only runs once, this
    // will avoid printing a message or running the same code multiple times
    timer.start(2000);
  }
  if(timer.active()){
    // you can also check if the timer is active at any point from another part of your code
  }
  // use timer.stop() to stop the timer and prevent execution.
  // use timer.remaining() to get the number of millis/micros remaining before it completes.
}
