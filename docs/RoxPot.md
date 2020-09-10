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

RoxPot pot;

void setup(){
  // for this example pin 0 and 1 on my teensy are connected to an pot
  pinMode(23, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxPot Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  pot.begin();
}

void loop(){
  // 2 parameters passed to update method

  // #1 the actual reading of the pot, in this case we passs the output of analogRead
  //    RoxPot doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxPot is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxPot and the class will do the job for you.

  // #2 (optional) debounce time in milliseconds, default: 1ms
  //    this value is used to debounce the pot, the potentiometer may have some
  //    noise to it and this can help remove some of it, add a 0.1uF capacitor
  //    between the pin and ground to remove additional noise
  //    Debouncing time is 8-bits so keep value at 0 to 255ms

  //    ROXPOT WILL LOWER THE RESOLUTION OF THE READING TO 7-BITS!!!
  if(pot.update(analogRead(23), 1)){
    uint8_t newPotReading = pot.read();
    Serial.print("Pot value: ");
    Serial.println(newPotReading);
  }
}
```
