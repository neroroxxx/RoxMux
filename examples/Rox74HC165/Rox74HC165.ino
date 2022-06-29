#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC165s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC165 <MUX_TOTAL> mux;


// pins for 74HC165
#define PIN_DATA  23 // pin 9 on 74HC165 (DATA)
#define PIN_LOAD  22 // pin 1 on 74HC165 (LOAD)
#define PIN_CLK   21 // pin 2 on 74HC165 (CLK))


// keep track of the previous state of the pins
// each element of the array holds the state of 8 pins, 1 per bit
bool pinState[MUX_TOTAL*8];

void setup(){
  Serial.begin(115200);
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
}

void loop(){
  // read the mux
  mux.update();
  for(uint8_t i=0, n=mux.getLength(); i < n ; i++){
    bool data = mux.read(i);
    if(data != pinState[i]){
      pinState[i] = data;
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(data);
    }
  }

}
