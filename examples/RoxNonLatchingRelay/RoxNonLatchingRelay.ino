/*
  RoxLatchingRelay lets you easily control a latching relay.
  You will still need to have the circuitry to set and reset the relay.

  UNLIKE THE OTHER ROX UTILITY CLASSES,
  THIS CLASS CURRENTLY DOES NOT SUPPORT MUX CONTROL
*/
#include <RoxMux.h>

// specify the time the coil needs to be energize
// refer to your relay's datasheet for that value
// this example uses a 50ms
// the second template argument is the state you want the pin to be to energize
// the coil, you can use LOW or HIGH, this is used to reverse the polarity
// for example, if you set this value to HIGH then the led will go HIGH when
// the coil needs to be energized and LOW when the coil is de-energized
RoxNonLatchingRelay <50, HIGH> relay;


void setup(){
  Serial.begin(115200);
  Serial.println("RoxNonLatchingRelay Example");
  // little delay before starting
  delay(100);
  // .begin()
  // first parameter is the pin for the relay
  // second parameter is the state the relay should be in at launch
  // you can use ROX_SET to energize the relay right away
  // or ROX_RESET to leave it de-energized
  relay.begin(15, ROX_RESET);
  // you can set the relay to momentary by passing true as the argument below
  relay.momentary(false);
}

void loop(){
  // always call the update() method in your loop
  // you can pass it a parameter in milliseconds, this parameter is used
  // for momentary hold, if you set the relay to momentary the relay
  // will switch to SET for the specified number of milliseconds then back to RESET
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
