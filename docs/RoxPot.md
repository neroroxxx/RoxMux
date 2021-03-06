*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxPot

RoxPot will read and debounce potentiometers either directly connected to a pin
or from a multiplexer output.

RoxPot lowers the resolution down to 7-bits so a value of 0 to 127 is returned.
Additionally RoxPot will debounce the reading (optional) to remove some unwanted
noise. Between the lower resolution and the debouncing, RoxPot can give you a pretty
accurate reading!

Give it a try and see for yourself!

```c++
#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC595s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
// blinking was added to version 1.1.5 of RoxMux
// BLINK_RATE determines the speed at which any led set to BLINK will turn on/off
// you have to use .blinkPin(pinNumber, true/false) to turn blinking on/off
// per pin, also an led will only blink if it's turned on
#define BLINK_RATE 50

Rox74HC595 <MUX_TOTAL, BLINK_RATE> mux;

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

unsigned long prevTime = 0;
uint8_t counter = 0;
const uint16_t totalPins = 8 * MUX_TOTAL;

// if you use a PWM pin you can set the brightness for all LEDs.
uint8_t brightness = 255;

void setup(){
  mux.begin(PIN_DATA, PIN_LATCH, PIN_CLK, PIN_PWM);
  prevTime = millis();

  // set the brightness, only works if you set and wired a PWM pin
  // 0 will turn them off, 255 will do a digitalWrite HIGH
  mux.setBrightness(brightness);

  // as of version 1.1.5 you can blink independent leds, you must set
  // the blink state via .blinkPin(pinNumber, true/false)
  // after that anytime you turn the led on, the led will blink instead of
  // just staying on.

  // You can set the blink rate as part of the instance declaration

  // set every other led to blink instead of staying on
  mux.blinkPin(0, true);
  mux.blinkPin(2, true);
  mux.blinkPin(4, true);
  mux.blinkPin(6, true);

}

void loop(){
  // the update() method will only write to the 74HC595 whenever a pin value
  // has changed from it's buffer.
  mux.update();

  // turn each led on then off every second
  if((millis()-prevTime) > 1000){
    // change the brightness every
    mux.allOff();
    mux.writePin(counter, HIGH);
    counter++;
    if(counter >= totalPins){
      counter = 0;
    }
    prevTime = millis();
  }
}

```
