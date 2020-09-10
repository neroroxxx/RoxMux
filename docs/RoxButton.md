*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxButton

RoxButton is a button debouncer that doesn't require you to pass the pin number.
Instead with RoxButton you pass it the state of the pin, the debounce time and
weather the button's reading will be Active Low or Active High.

RoxButton will let you read if the button was pressed(), released() or held(milliseconds).

Give it a try and see for yourself!

```c++
#include <RoxMux.h>

RoxButton button;

void setup(){
  // for this example pin 0 on my teensy is connected to a momentary button
  pinMode(0, INPUT_PULLUP);
  // pin 13 is the default led it will light up the led when the button is pressed
  pinMode(13, OUTPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxButton Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  button.begin();
}

void loop(){
  // 3 parameters passed to update method

  // #1 the actual state of the button, in this case we passs the output of digitalRead
  //    RoxButton doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxButton is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxButton and the class will do the job for you.
  //    ROXBUTTON WILL DEBOUNCE THE READING, DO NO DEBOUNCE IT YOURSELF.

  // #2 the debounce time, this value is in milliseconds, default 50ms
  //    this is how long to wait before reading the button again

  // #3 the state of the button when it's active, if your pin has a pullup resisitor
  //    then the active state will be LOW, if it's a pulldown resistor then the
  //    active state is HIGH.
  button.update(digitalRead(0), 50, LOW);
  // the .held() function can take the time in milliseconds that you want to hold
  // the button for, if no value is passed the default 500ms will be used
  if(button.held()){
    Serial.println("* Button has been held for 500 milliseconds");
  } else if(button.pressed()){
    Serial.println("+ Button has been pressed");
  } else if(button.released(true)){
    // you can pass a boolean (true or false) to the .released() method
    // this boolean will determine if the release should be triggered after
    // a held() has been triggered.
    Serial.println("- Button has been released");
  }
  // if you only want to know if the button was held then
  digitalWrite(13, button.getCurrentState());
}
```
