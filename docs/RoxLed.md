*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxLed

RoxLed is a simple utility to control an led, in it's most basic form it will
let you turn LEDs on/off without constantly calling the digitalWrite function.

In addition, RoxLed lets you easily blink and pulse an LED without the use of delay()
RoxLed can also be used with the RoxMCP2301X and Rox74HC595.

A RoxLed object must be created for each LED, RoxLed has 3 modes

**ROX_DEFAULT** turn an LED ON/OFF
**ROX_BLINK** blink an LED at the specified rate
**ROX_PULSE** pulse an LED (turn it on for 75ms then off) every x milliseconds

See the examples for more info


```c++
#include <RoxMux.h>

// for this example we'll use the built-in led on pin 13
RoxLed led;

// used to change modes
unsigned long prevTime = 0;
uint8_t mode = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("RoxLed Example");
  // little delay before starting
  delay(100);
  // begin() will take in the pin number, if you will use it with a mux then
  // leave the field blank.
  // we'll use the built-in led pin 13
  led.begin(13);
  // you can set a mode for the led when it's on, there are 3 modes
  // default (led on or off), blink and pulse.
  // you can set the mode here and change this anywhere in your code

  // DEFAULT: in this mode when you call the .on() method the led
  // will turn on and stay on until you call the .off() method
  led.setMode(ROX_DEFAULT);

  // BLINK: in this mode when you call the .on() method the led
  // will turn on and off repeately until you call the .off() method
  // you can set the rate at which the led turns on/off in the .update() method
  //led.setMode(ROX_BLINK);

  // PULSE: this method is similar to BLINK but instead the led will light up
  // for 75 milliseconds, then turn off and repeat based on the specified rate.
  // For PULSE the rate parameter passed to the .update() method will instead
  // be used for the next time the led will pulse again.
  // for example the pulse method can be used for TEMPO, if you pass a value of
  // 500 to the .update() method then the led will turn on every 500 milliseconds
  // but only for 75 milliseconds at a time, where the BLINK method will
  // keep the led on for 500ms then off for 50ms and so on.
  //led.setMode(ROX_PULSE);

  prevTime = millis();

  // turn the led on
  led.on();
}

void loop(){
  // the .update() method takes in a rate, this rate is used for blink and pulse.
  // the .update() method will also return a byte from 0 to 2 where:
  // 0 - no change to the state of the led.
  // 1 - the led has been turned off (only returned once when the state changes)
  // 2 - the led has been turned on (only returned once when the state changes)

  // for this example tho we'll change between modes every 2 seconds
  if((unsigned long) millis()-prevTime >= 5000){
    if(mode>=2){
      mode = 0;
    } else {
      mode++;
    }
    led.setMode(mode);
    prevTime = millis();
  }

  if(mode==ROX_DEFAULT){
    led.update();
  } else if(mode==ROX_BLINK){
    // turn led on/off for 50ms at a time
    led.update(50);
  } else if(mode==ROX_PULSE){
    // light up led every 500ms
    led.update(500);
  }
  // You can type anything in the serial monitor to toggle the led on/off
  // make sure you hit send and you specify the right baud rate in setup
  bool toggle = false;
  while(Serial.available() > 0){
    Serial.read();
    toggle = true;
  }
  if(toggle){
    bool state = led.toggle();
    Serial.print("toggled: ");
    Serial.println(state ? "ON" : "OFF");
  }
  // anytime you want to turn the led off just call this method
  // next time you call .on() the led will return to it's previous mode
  // led.off();
}
```
