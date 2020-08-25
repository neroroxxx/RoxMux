*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

# RoxMux

RoxMux is a Collection of Multiplexer Controllers for some commonly used multiplexer chips.

RoxMux includes support for:

- [74HC165](docs/74HC165.md)
- [74HC595](docs/74HC595.md)
- [74HC4067](docs/74HC4067.md)
- [74HC4051](docs/74HC4067.md)

See examples for code information.

RoxMux containes multiple libraries in one, just include `<RoxMux.h>` to get started. All the libraries are templated to reduce RAM usage.


### 74HC595
This is a simple example to write to a 74HC595, change `MUX_TOTAL` to the number of chainged 74HC595s you have wired.

```c++
#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC595s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC595 <MUX_TOTAL> mux;

// pins for 74HC595
#define PIN_DATA    11 // pin 14 on 74HC595 (DATA)
#define PIN_LATCH   9  // pin 12 on 74HC595 (LATCH)
#define PIN_CLK     8  // pin 11 on 74HC595 (CLK)

// PIN_PWM MUST BE A PWM CAPABLE PIN
// set it's value to -1 if you do not wish to control the brightness
// if you don't need to control the brightness you must wire
// pin 13 on the 74HC595 to ground.
#define PIN_PWM     10  // pin 13 on 74HC595

// Wire pin 11 to VCC

unsigned long prevTime = 0;
uint8_t counter = 0;
const uint16_t totalPins = 8 * MUX_TOTAL;

// if you use a PWM pin you can set the brightness for all LEDs.
uint8_t brightness = 255;

void setup(){
  mux.begin(PIN_DATA, PIN_LATCH, PIN_CLK, PIN_PWM);
  // set the brightness, only works if you set and wired a PWM pin
  // 0 will turn them off, 255 will do a digitalWrite HIGH
  mux.setBrightness(brightness);
  // used for timing so we don't have to use a delay()
  prevTime = millis();
}

void loop(){
  // the update() method will only write to the 74HC595 whenever a pin value
  // has changed from it's buffer. Always call this method in the loop()
  mux.update();

  // turn each led on then off every 500 milliseconds
  if((millis()-prevTime) > 500){
    // turn off all LEDs in the buffer
    mux.allOff();
    // now turn on an led, writing to the 595 will only take place when mux.update()
    // is called, this way you can update as many LEDs then draw them.
    mux.writePin(counter, HIGH);

    counter++;
    if(counter >= totalPins){
      counter = 0;
    }
    prevTime = millis();
  }
}
```


### 74HC195
This is a simple example to read from a 74HC165, change `MUX_TOTAL` to the number of chainged 74HC165s you have wired.

```c++
#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC165s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC165 <MUX_TOTAL> mux;

#define PIN_DATA  23 // pin 9 on 74HC165 (DATA)
#define PIN_LOAD  22 // pin 1 on 74HC165 (LOAD)
#define PIN_CLK   21 // pin 2 on 74HC165 (CLK))

// keep track of the previous state of the pins
// each element of the array holds the state of 8 pins, 1 per bit
uint8_t pinStates[MUX_TOTAL];

void setup(){
  Serial.begin(115200);
  mux.begin(PIN_DATA, PIN_LOAD, PIN_CLK);
}

void loop(){
  // read all the mux chips at once, always call it in the loop()
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
```


### 74HC4067
This is a simple example to read from a 74HC4067, change `MUX_TOTAL` to the number of chainged 74HC4067 you have wired.

```c++
#include <RoxMux.h>

// MUX_TOTAL is the number of 74HC4067s that you have chained together
// if you have more than 1 then change it to that number.
#define MUX_TOTAL 1
Rox74HC4067 <MUX_TOTAL> mux;

#define PIN_SIG  23
#define PIN_CH0  22
#define PIN_CH1  21
#define PIN_CH2  20
#define PIN_CH3  19

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
  mux.begin(PIN_CH0, PIN_CH1, PIN_CH2, PIN_CH3);
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
  for(uint8_t i=0,n=MUX_TOTAL*16;i<n;i++){
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
```

### 74HC4051
This is a simple example to read from a 74HC4051, change `MUX_TOTAL` to the number of chainged 74HC4051 you have wired.

```c++
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
```
