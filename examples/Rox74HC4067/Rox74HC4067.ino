/*
  https://www.RoxXxtar.com/bmc
  Example to read 2 74HC4067
  The Rox74HC40XX has 3 classes, a base Rox74HC40XX and 2 children:

  Rox74HC4067, this is a 16 pin analog-in multiplexer
  Rox74HC4051, this is a 8 pin analog-in multiplexer
*/

#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC4067s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 2
Rox74HC4067 <MUX_TOTAL> mux;


#define PIN_S0  23
#define PIN_S1  22
#define PIN_S2  21
#define PIN_S3  20
#define PIN_SIG0  19
#define PIN_SIG1  18

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
  mux.begin(PIN_S0, PIN_S1, PIN_S2, PIN_S3);
  // set what pin is the signal pin for the first mux
  // the first argument is the index of the mux, second is the pin number
  mux.setSignalPin(0, PIN_SIG0);
  // second mux signal pin
  mux.setSignalPin(1, PIN_SIG1);
}

void loop(){
  // read the mux
  // this method should always be in your loop() function
  // the update method has an option parameter which is the number of millis
  // between each pin read, if you don't pass an argument it will read them
  // every 1 millisecond
  mux.update();
  // read each pin
  for(uint8_t i=0,n=mux.getLength();i<n;i++){
    // since the Rox74HC4067 doesn't smooth the values it just reads them raw
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
