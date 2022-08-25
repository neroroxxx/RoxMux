// This example is for the BadassMIDI.com OCTOSWITCH
// Each OCTOSWITCH has 8 buttons, you can chain multiple OCTOSWITCH boards in series
// OPEN THE SERIAL MONITOR TO VIEW the change of states of pins.
#include <RoxMux.h>

// pins for 74HC165
#define PIN_DATA  23 // pin 9 on MUXIN-16 (DAT)
#define PIN_LOAD  22 // pin 1 on MUXIN-16 (LD)
#define PIN_CLK   21 // pin 2 on MUXIN-16 (CLK))

// OCTO_TOTAL (required) is the number of OCTOSWITCH boards that you have chained together
// if you have more than 1 then change it to that number.
#define OCTO_TOTAL 1
// BTN_DEBOUNCE (optional) the debounce time for buttons, default is 50
// however you can change this if needed
#define BTN_DEBOUNCE 50
// add the mux, you only need one object if they are all chained together.
RoxOctoswitch <OCTO_TOTAL, BTN_DEBOUNCE> octoswitch;

void setup(){
  Serial.begin(115200);
  // set the pins
  octoswitch.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
  octoswitch.setCallback(onButtonPress);

  // Other options
  // allow double pressing a specific button by using the function below
  // the first parameter is the index of the button
  // the second parameter is the threshold which is the time in milliseconds
  // between presses to wait before handling a double press
  // octoswitch.setDoublePressThreshold(0, 300);

  // use the function below if you want a specific button to ignore the released
  // event after the HOLD event was triggered
  // the first parameter is the index of the button
  // the second parameter is true or false. true will ignore the release event
  // octoswitch.setIgnoreAfterHold(0, true);
}
void loop(){
  // read the mux and it's buttons
  // you can pass the hold time to trigger a hold in milliseconds
  // this parameter is optional and defaults to 500ms
  octoswitch.update(500);
}
// this callback is triggered whenever a button is pressed
// @btnIndex the index of the button, buttons are index from 0 to the total
// number of buttons in all the OCTOSWITCH boards, for example, if you have
// a total of 3 OCTOSWITCH boards chainged together, the first button of your
// first OCTOSWITCH will be btnIndex 0,
void onButtonPress(uint16_t btnIndex, uint8_t btnType){

  // to check if a specific button was pressed
  if(btnIndex==0 && btnType==ROX_PRESSED){
    // button 0 was pressed
    // do something
  }

  Serial.print("Button # ");
  Serial.print(btnIndex);
  Serial.print(" ");

  if(btnType == ROX_HELD){
    Serial.println("Held");
  } else if(btnType == ROX_DOUBLE){
    Serial.println("Double Pressed");
  } else if(btnType == ROX_PRESSED){
    Serial.println("Pressed");
  } else if(btnType == ROX_RELEASED){
    Serial.println("Released");
  }
}
