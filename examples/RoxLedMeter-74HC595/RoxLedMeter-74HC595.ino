#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC595s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
// blinking was added to version 1.1.5 of RoxMux
// BLINK_RATE determines the speed at which any led set to BLINK will turn on/off
// you have to use .blinkPin(pinNumber, true/false) to turn blinking on/off
// per pin, also an led will only blink if it's turned on
#define BLINK_RATE 50

// initialize the mux
Rox74HC595 <MUX_TOTAL, BLINK_RATE> mux;

// How many leds will be used for the vu meter
#define TOTAL_LEDS 8
// this library requires all pins to be wired incrementally,
// in other words if you have 4 pins, the first pin can be pin 0, then
// all other pins will have to be 1, 2 and 3, if your first pin is 5
// then the other 3 pins will have to be 6, 7 and 8.
// the FROM_PIN is the lowest "LEVEL" led
// for this example wire 4 leds to pins 0, 1, 2 and 3 of the 74HC595
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
RoxVuMeter <TOTAL_LEDS, FROM_PIN, RANGE_MIN, RANGE_MAX> ledMeter;

// pins for 74HC595
#define PIN_DATA    11 // pin 14 on 74HC595 (DATA)
#define PIN_LATCH   9  // pin 12 on 74HC595 (LATCH)
#define PIN_CLK     8  // pin 11 on 74HC595 (CLK)

// PIN_PWM MUST BE A PWM CAPABLE PIN
// set it's value to -1 if you do not wish to control the brightness
// if you don't need to control the brightness you must wire
// pin 13 on the 74HC595 to ground.
#define PIN_PWM     10  // pin 13 on 74HC595

// Wire pin 10 to VCC

// for timing
RoxEndlessTimer timer;

// if you use a PWM pin you can set the brightness for all LEDs.
uint8_t brightness = 255;

void setup(){
  // the library uses a callback to tell your sketch which leds need to be
  // turned on/off the library itself will not deal with the pins so you must
  // do that as part of your sketch
  mux.begin(PIN_DATA, PIN_LATCH, PIN_CLK, PIN_PWM);

  // set the brightness, only works if you set and wired a PWM pin
  // 0 will turn them off, 255 will do a digitalWrite HIGH
  mux.setBrightness(brightness);

  // add the callback to ledMeter
  ledMeter.onUpdate(ledsUpdated);

  // set the RoxTimer to loop every 200 milliseconds
  timer.start(200);
}
// this is the callback that will tell us when and what leds need to be written
void ledsUpdated(uint16_t pinNumber, bool state){
  // the callback will return the actual pin number and the state the led
  // must be updated to.
  // the library will only tell you to write to a pin if that pin needs to be updated
  // in other words, if you go from 2 leds being on to 4 leds being on, the library
  // will then tell you to turn on the last 2 leds and so on.
  // this is to avoid writing the same state to the pins
  mux.writePin(pinNumber, state);
}
void loop(){
  // the update() method will only write to the 74HC595 whenever a pin value
  // has changed from it's buffer.
  mux.update();
  // assign a random value to the VU Meter
  if(timer.complete()){
    Serial.print(".");
    // generate a random number between the range that was specified
    uint8_t newValue = random(RANGE_MIN, RANGE_MAX+1);
    ledMeter.setValue(newValue);
  }
}
