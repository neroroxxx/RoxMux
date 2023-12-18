*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxFadingLed

RoxFadingLed is a simple utility to fade an Led, you can turn the led on/off when it's on
the led will fade a specified rate (in beats per minutes / BPM)

RoxFadingLed requires the LED to be wired to a PWM pins.

See the examples for more info


```c++
#include <RoxMux.h>

// for this example we'll use the pin 3 which is a PWM pin on teensy
RoxFadingLed led;

void setup(){
  Serial.begin(115200);
  Serial.println("RoxFadingLed Example");
  // little delay before starting
  delay(100);

  // begin() will take in the pin number
  // for this example we'll use the pin 3 which is a PWM pin on teensy
  led.begin(3);

  // the led fading rate is handled in Beats per Minute
  // use .setBPM with a value between 30 and 300
  // the lower the value the slower the fading rate
  led.setBPM(120);

  // turn the led on
  led.on();
}

void loop(){
  // use led.on(), led.off() or led.toggle() to control the led
  // the led will transition it's brightness from 0 to 255 
  // then back down to 0 by 1 value.
  led.update();
}

```
