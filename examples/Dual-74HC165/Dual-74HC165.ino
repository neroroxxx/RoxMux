#include <RoxMux.h>

// MUX_TOTAL is the number of BMC CORE Dual 74HC165 Breakout boards that you have chained
// together, if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
RoxDual74HC165 <MUX_TOTAL> mux;

#define PIN_DATA  23 // pin 9 on 74HC165 (DATA)
#define PIN_LOAD  22 // pin 1 on 74HC165 (LOAD)
#define PIN_CLK   21 // pin 2 on 74HC165 (CLK)

// keep track of the previous state of the pins
// each element of the array holds the state of 8 pins, 1 per bit
uint16_t pinStates[MUX_TOTAL];

void setup(){
  Serial.begin(115200);
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
}

void loop(){
  // read the mux
  mux.update();
  for(uint8_t i=0;i<MUX_TOTAL;i++){
    // "i" specifies which mux to read
    uint16_t data = mux.readPins(i);
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
  Serial.print(n);
  Serial.print(" >>>");
  for(uint8_t i=0;i<16;i++){
    Serial.print(bitRead(pinStates[n], i));
  }
  Serial.println("");
  Serial.println("------------");
}
