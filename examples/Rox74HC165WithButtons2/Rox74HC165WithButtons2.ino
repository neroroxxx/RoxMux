#include <RoxMux.h>
// This is an alternate example of Rox74HC165WithButtons
// This example will show you how to use the Rox74HC165 class
// with the RoxButton class to use multiple buttons using 
// only some of the pins of the 165.
// Please refer to the Rox74HC165 and RoxButton examples
// for more details on each class.

// MUX_TOTAL is the number of 74HC165s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC165 <MUX_TOTAL> mux;

// specify the number of buttons you will be using
// remember the 74HC165 has 8 pins so you can have
// up to 8 buttons per 74HC165
// for this example we'll use 4 buttons
// we'll use pins 0, 1, 6, 7, pins 2,3,4,5 will be
// used for other things not buttons.
// note that you will have the same number of
// pins in the array as the number of buttons.
#define NUM_BUTTONS 4
uint8_t pinsToUse[NUM_BUTTONS] = {0, 1, 6, 7};
RoxButton buttons[NUM_BUTTONS];


// update with your pins for the 74HC165
#define PIN_DATA  23 // pin 9 on 74HC165 (DATA)
#define PIN_LOAD  22 // pin 1 on 74HC165 (LOAD)
#define PIN_CLK   21 // pin 2 on 74HC165 (CLK))


void setup(){
  Serial.begin(115200);
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
  // since we have an array of RoxButton we'll initialize
  // each one in a "for" loop
  // we don't need to deal with pin numbers here.
  for(int i = 0 ; i < NUM_BUTTONS; i++){
    // the .begin() method starts the debouncing timer
    buttons[i].begin();

    // if you'll allow double press on the buttons you
    // can uncomment this line, see the RoxButton example
    // for more details.
    // buttons[i].setDoublePressThreshold(300);
  }
}

void loop(){
  // read the mux
  mux.update();
  for(int i = 0 ; i < NUM_BUTTONS; i++){
    // read each button
    // in this case we will only use the pins in the
    // pinsToUse array, button 0 will correspond to
    // the first element in the pinsToUse array.
    // you can re-arrange the pinsToUse array any
    // way you want to match your buttons layout
    buttons[i].update(mux.read(pinsToUse[i]), 50, LOW);

    // inside the loop you can immediately check
    // if the button was held, pressed, etc.
    if(buttons[i].held()){

      Serial.println("* Button has been held for 500 milliseconds");

    } else if(buttons[i].doublePressed()){

      // NOTE: when you first press a release a button both the press and release
      // will be triggered, however if you press the button a second time within
      // the default 300ms time for double press, neither the press nor the release
      // will be triggered, only the double press,
      // if you want to trigger the release of the button after a double press
      // you can pass true to the .doublePressed(true) function
      Serial.println("& Button has been double pressed");

    } else if(buttons[i].pressed()){

      Serial.println("+ Button has been pressed");

    } else if(buttons[i].released(true)){
      // you can pass a boolean (true or false) to the .released() method
      // true if you want to ignore the "released" reading after "held" was triggered
      Serial.println("- Button has been released");
    }
  }
}
