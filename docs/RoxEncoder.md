*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxEncoder

RoxEncoder reads and debounces encoders, you can use the readings for your pins
or a Multiplexer output.

Unlike the typical encoder library, RoxEncoder doesn't keep track of a "value"
instead it just tells you if the encoder was rotated clockwise or counter-clockwise.
Then you are responsible for keeping track of what changes based on the direction
of the rotation.

Give it a try and see for yourself!

```c++

#include <RoxMux.h>

RoxEncoder encoder;

void setup(){
  // for this example pin 0 and 1 on my teensy are connected to an encoder
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxEncoder Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  encoder.begin();
}

void loop(){
  // 3 parameters passed to update method

  // #1 the actual state of the encoder Pin A, in this case we passs the output of digitalRead
  //    RoxEncoder doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxEncoder is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxEncoder and the class will do the job for you.
  //    ROXENCODER WILL DEBOUNCE THE READING, DO NO DEBOUNCE IT YOURSELF.

  // #2 same as #1 but for Pin B of the encoder

  // #3 the debounce time, this value is in milliseconds, default 1ms
  //    this is how long to wait before reading the encoder again
  //    this is needed to clean some of the noise on the encoders.
  //    this value is 8-bits so keep it between 0 and 255

  // #4 the state of the encoder pins when they're active,
  //    if your pin has a pullup resisitor then the active state will be LOW,
  //    if it's a pulldown resistor then the active state is HIGH.
  encoder.update(digitalRead(0), digitalRead(1), 2, LOW);

  // the update method will read the encoder and if can be used to test if
  // there was movement, however you can also use .read() after the update
  // to check if there was movement

  // .read() checks if the encoder was rotated
  if(encoder.read()){
    // encoder has been rotated
    Serial.print("Encoder rotated ");
    // .increased() tells us if the encoder was rotated clockwise
    // you can also use .clockwise()
    // if this method returns false the encoder was rotated counter-clockwise
    if(encoder.increased()){
      Serial.println("Clockwise");
    } else {
      Serial.println("Counter-Clockwise");
    }
  }
}
```
