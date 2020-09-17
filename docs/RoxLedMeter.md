*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxLedMeter

RoxLedMeter makes it really easy to create your own VU Meter using Leds.

All Leds must be connected to pins in assending order, that is if you start at pin 5 and you have 9 leds then the first led will be pin 5, second must be pin 6, third must be pin 7 and fourth must be pin 8, where the first pin is the lowest value on the VU Meter and the last pin is the Highest/Loudest/etc.

RoxLedMeter is meant to be used either with built-in pins or Multiplexer pins, for this reason it works with a callback which tells your sketch what leds to turn on/off, this callback will have your digitalWrite call.

Only leds that must be updated will be passed by the callback that way we avoid unnecessary use of digitalWrite().

Give it a try and see for yourself!

```c++
// THIS EXAMPLE USES DIRECT PINS ON YOUR ARDUINO

#include <RoxMux.h>
// How many leds will be used
#define TOTAL_LEDS 4
// this library requires all pins to be wired incrementally,
// in other words if you have 4 pins, the first pin can be pin 0, then
// all other pins will have to be 1, 2 and 3, if your first pin is 5
// then the other 3 pins will have to be 6, 7 and 8.
// the FROM_PIN is the lowest "LEVEL" led
// for this example wire 4 leds to pins 0, 1, 2 and 3
// where pin 0 is the lowest value and pin 3 is your highest/loudest/clipping etc.
#define FROM_PIN 0
// RANGE_MIN and RANGE_MAX tell the library what value range you will pass to it
// in other words, this example only has a VU Meter with 4 leds, however you
// may be using an audio or other input that will give you a value from 0 to 100
// where 0 is no sound and 100 is loudest, so you have to tell the library
// what this range is, the library will then map the value so when the value is 0
// all 4 leds are off, if value is 50 then 2 leds are on
// if value is 75 then 3 leds will be on and if value is 100 all 4 leds will be on
// for this example we'll use a value from 0 to 10
#define RANGE_MIN 0
#define RANGE_MAX 10

// initialize class
RoxLedMeter <TOTAL_LEDS, FROM_PIN, RANGE_MIN, RANGE_MAX> ledMeter;

// for timing...
unsigned long prevTime = 0;

void setup(){
  // the library uses a callback to tell your sketch which leds need to be
  // turned on/off the library itself will not deal with the pins so you must
  // do that as part of your sketch
  for(uint8_t i=0;i<TOTAL_LEDS;i++){
    pinMode(i+FROM_PIN, OUTPUT);
    digitalWrite(i+FROM_PIN, LOW);
  }
  // add the callback to ledMeter
  ledMeter.onUpdate(ledsUpdated);
  prevTime = millis();
}

// this is the callback that will tell us when and what leds need to be written
void ledsUpdated(uint16_t pinNumber, bool state){
  // the callback will return the actual pin number and the state the led
  // must be updated to.
  // the library will only tell you to write to a pin if that pin needs to be updated
  // in other words, if you go from 2 leds being on to 4 leds being on, the library
  // will then tell you to turn on the last 2 leds and so on.
  // this is to avoid writing the same state to the pins
  digitalWrite(pinNumber, state);
}

void loop(){
  // assign a random value to the VU Meter every 100 milliseconds
  if((millis()-prevTime) > 100){
    // generate a random number between the range that was specified
    uint8_t newValue = random(RANGE_MIN, RANGE_MAX+1);
    ledMeter.setValue(newValue);
    prevTime = millis();
  }
}
```
