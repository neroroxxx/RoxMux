/*
  RoxButton is different from must button reading libraries because it's meant
  to be used with either the Pin on your arduino or a value read from a multiplexer

  Because of this you do not have to tell RoxButton what pin the button is connected
  to, instead you give RoxButton the state of your button, in other words, you
  read the pin state and RoxButton handles debouncing and determining the state.

  Because of this you must add the pinMode for your pin in your setup code

  You can also look at RoxButton as a debouncer for a noisy digital signal
  that will also tell you if the signal was held in the active state.
*/
#include <RoxMux.h>

RoxButton button;

void setup(){
  // for this example pin 0 on my teensy is connected to a momentary button
  pinMode(0, INPUT_PULLUP);
  // pin 13 is the default led it will light up the led when the button is pressed
  pinMode(13, OUTPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxButton Example for Latching Buttons (non-momentary)");
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

  // #2 (optional) the debounce time, this value is in milliseconds, default: 50ms
  //    this is how long to wait before reading the button again

  // #3 (optional) the state of the button when it's active,
  //    if your pin has a pullup resisitor then the active state will be LOW,
  //    if it's a pulldown resistor then the active state is HIGH.
  button.update(digitalRead(0), 50, LOW);

  if(button.latched()){

    Serial.println("* Button is Latched (closed)");

  } else if(button.unlatched()){

    Serial.println("* Button is Unlatched (open)");

  }
  // if you only want to know if the button was held then
  digitalWrite(13, button.getCurrentState());
}
