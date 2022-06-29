// This example is for the BadassMIDI.com MUXIN-16
// Each MUXIN-16 has 16 pins, you can wire buttons to any of these pins.
// OPEN THE SERIAL MONITOR TO VIEW the change of states of pins.
#include <RoxMux.h>

// pins for 74HC165
#define PIN_DATA  23 // pin 9 on MUXIN-16 (DAT)
#define PIN_LOAD  22 // pin 1 on MUXIN-16 (LD)
#define PIN_CLK   21 // pin 2 on MUXIN-16 (CLK))

// MUX_TOTAL is the number of MUXIN-16 that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
// add the mux, you only need one object if they are all chained together.
RoxMUXIN16 <MUX_TOTAL> mux;
// the number of buttons is equal to the number of MUXIN-16s multiplied by 16
RoxButton button[MUX_TOTAL*16];

void setup(){
  Serial.begin(115200);
  // set the pins
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
  // initialize the buttons
  // mux.getLenth() returns the total number of pins
  for(uint8_t i=0, n=mux.getLength(); i < n ; i++){
    button[i].begin();
  }
}

void loop(){
  // read the mux
  mux.update();
  // read the buttons.
  for(uint8_t i=0, n=mux.getLength(); i < n ; i++){
    button.update(mux.read(i), 50, LOW);
    if(button.held()){
      Serial.print(i);
      Serial.println(" held for 500 milliseconds");
    } else if(button.pressed()){
      Serial.print(i);
      Serial.println(" Pressed");
    } else if(button.released(true)){
      Serial.print(i);
      Serial.println(" Released");
    }
  }

}
