/*
  https://www.RoxXxtar.com/bmc
  Example to read one 74HC4051
  The Rox74HC40XX has 3 classes, a base Rox74HC40XX and 2 children:

  Rox74HC4067, this is a 16 pin analog-in multiplexer
  Rox74HC4051, this is a 8 pin analog-in multiplexer
*/

#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC4051s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC4051 <MUX_TOTAL> mux;

#define PIN_SIG  23
#define PIN_CH0  22
#define PIN_CH1  21
#define PIN_CH2  20

// pins used during development
//#define PIN_SIG  23
//#define PIN_CH0  31
//#define PIN_CH1  30
//#define PIN_CH2  22

// this is the pin of the 74HC4067 that we will read
#define PIN_TO_READ 0

// keep track of the previous value, while the library handles 10-bit values
// for this example we'll lower the resolution to 8-bits
// this is to avoid some of the noise since there's no smoothing in the library.
uint8_t prev = 0;

void setup(){
  delay(100);
  Serial.begin(115200);
  // being the mux
  mux.begin(PIN_CH0, PIN_CH1, PIN_CH2);
  // set what pin is the signal pin for the first mux
  // the first argument is the index of the mux, second is the pin number
  mux.setSignalPin(0, PIN_SIG);
}

void loop(){
  // read the mux
  // this method should always be in your loop() function
  // the update method has an option parameter which is the number of millis
  // between each pin read, if you don't pass an argument it will read them
  // every 1 millisecond
  mux.update();
  // read each pin
  for(uint8_t i=0,n=MUX_TOTAL*8;i<n;i++){
    // since the Rox74HC4051 doesn't smooth the values it just reads them raw
    // we'll only read one pin on this example
    if(i==PIN_TO_READ){
      uint8_t data = mux.read(i)>>2;
      if(data!=prev){
        prev = data;
        Serial.print("Mux pin ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(data);
      }
    }
  }
}
