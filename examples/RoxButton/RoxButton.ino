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
  Serial.println("RoxButton Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  button.begin();
  // if you will be using double press then you can set the time interval between
  // pressed in milliseconds, in the line below we set it to 300ms which is the
  // default value.
  // the double press is triggered based on the first release and next press of the button
  // that is if the button is currentley in it's open state, and it changes to
  // it's closed state, as soon as it goes back to the open state a flag is set
  // in which case the program will wait for the next time the button goes into
  // it's closed state, as soon as the button goes into the closed state then
  // another check is performed in which case if that 2nd pressed happened in less
  // than 300 milliseconds then that's a double press.
  button.setDoublePressThreshold(300);
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

  // the .held() function can take the time in milliseconds that you want to hold
  // the button for, if no value is passed the default 500ms will be used
  if(button.held()){

    Serial.println("* Button has been held for 500 milliseconds");

  } else if(button.doublePressed()){

    // NOTE: when you first press a release a button both the press and release
    // will be triggered, however if you press the button a second time within
    // the default 300ms time for double press, neither the press nor the release
    // will be triggered, only the double press,
    // if you want to trigger the release of the button after a double press
    // you can pass true to the .doublePressed(true) function
    Serial.println("& Button has been double pressed");

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
