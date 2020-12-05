#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC165s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC165 <MUX_TOTAL> mux;

#define PIN_DATA  21 // pin 9 on 74HC165 (DATA)
#define PIN_LOAD  22 // pin 1 on 74HC165 (LOAD)
#define PIN_CLK   23 // pin 2 on 74HC165 (CLK))

// keep track of the previous state of the pins
// each element of the array holds the state of 8 pins, 1 per bit
uint8_t pinStates[MUX_TOTAL];

void setup(){
  Serial.begin(115200);
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
}

void loop(){
  // read the mux
  mux.update();
  for(uint8_t i=0;i<MUX_TOTAL;i++){
    // read all pins from the mux specified by "i"
    uint8_t data = mux.read(i);
    // to read a single pin from a specific mux you would do:
    // bool data = mux.read(muxIndex, muxPin);
    // where muxIndex is the mux you are reading from
    // and muxPin is the pin of that mux you want to read


    // compare the mux states to the last saved states
    if(data != pinStates[i]){
      // if they change update the last states and print their values
      pinStates[i] = data;
      printStates(i);
    }
  }
}
void printStates(uint8_t n){
  Serial.print("Mux: ");
  Serial.println(n);
  for(uint8_t i=0;i<8;i++){
    Serial.print(bitRead(pinStates[n], i));
  }
  Serial.println("");
  Serial.println("------------");
}
