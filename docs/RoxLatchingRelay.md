*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxLatchingRelay

RoxLatchingRelay lets you easily control Latching Relays (2 coil relays).

***You will need to have the proper circuitry for your relay***

Most mechanical relays need to be energized for a minimum period of time, RoxLatchingRelay handles that part for you, the relay energizing is non-blocking (no delay() function used).

RoxLatchingRelay requires 2 pins and you can reverse the polarity of the pins, You can also set the coils to be energized momentarily.

See the examples for more info


```c++
#include <RoxMux.h>

// specify the time the coil needs to be energize
// refer to your relay's datasheet for that value
// this example uses a 50ms
// the second template argument is the state you want the pin to be to energize
// the coil, you can use LOW or HIGH, this is used to reverse the polarity
// for example, if you set this value to HIGH then the led will go HIGH when
// the coil needs to be energized and LOW when the coil is de-energized
RoxLatchingRelay <50, HIGH> relay;


void setup(){
  Serial.begin(115200);
  Serial.println("RoxLatchingRelay Example");
  // little delay before starting
  delay(100);
  // begin() will take in the RESET pin, the SET pin and weather the relay
  // should be in SET or RESET state at launch
  // you may leave that field blank if you don't want to change from whatever
  // the initial state is.
  relay.begin(15, 16, ROX_RESET);
  // you can set the relay to momentary by passing true as the argument below
  relay.momentary(false);
}

void loop(){
  // always call the update() method in your loop
  // you can pass it a parameter in milliseconds, this parameter is used
  // for momentary hold, if you set the relay to momentary the relay
  // will switch to the specified position (set or reset) then after
  // the specified number of milliseconds it will switch to it's oposite position
  // this is great for things like Guitar Amp Switching.
  relay.update(100);
  // TYPE ANYTHING ON THE SERIAL MONITOR TO TOGGLE BETWEEN SET OR RESET
  bool toggle = false;
  while(Serial.available() > 0){
    Serial.read();
    toggle = true;
  }
  if(toggle){
    relay.toggle();
  }
}

```
