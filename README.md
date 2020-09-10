*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

# RoxMux

RoxMux is a Collection of Multiplexer Controllers for some commonly used multiplexer chips.

RoxMux includes support for:

- [74HC165](docs/74HC165.md)
- [74HC595](docs/74HC595.md)
- [74HC4067](docs/74HC4067.md)
- [74HC4051](docs/74HC4067.md)
- [MCP23017](docs/MCP23017.md) *(supported as of version 1.1.4)*
- [MCP23018](docs/MCP23017.md) *(supported as of version 1.1.4)*

In Addition as of Version 1.1.3 RoxMux has a class to read buttons called RoxButton. This class
allows you to read a button connected directly to a pin or a multiplexer.
RoxMux also allows you to read/debounce and detect a Press, Release and if the button
was held!

Version 1.1.4 adds support for encoders and pots reading.

- [RoxButton](docs/RoxButton.md) *(supported as of version 1.1.3)*
- [RoxEncoder](docs/RoxEncoder.md) *(supported as of version 1.1.4)*
- [RoxPot](docs/RoxPot.md) *(supported as of version 1.1.4)*

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

### MCP23017 or MCP23018
```c++
#include <RoxMux.h>

// 0x20 is the address of the MCP2301X chip
// if you plan on using multiple MCPs just create an instance for each one
// and pass the address
// change to RoxMCP23018 if you want for the MCP23018 tho both work with the same code
RoxMCP23017 <0x20> mux;

void setup(){
  // begin the mux, the parameter passed determines the i2c speed, normal or fast
  // true is fast @ 400,000MHz, false is normal @ 100,000MHz
  mux.begin(true);
  // ALWAYS CALL .pinMode() in your setup as it's only implemented once.
  // a pin can not change it's mode on the fly only before the first time
  // .update() is called

  // set the first 8 pins as output and connect leds with resistors
  mux.pinMode(0, OUTPUT); // pin 0 = GPA0 of MCP2301X
  mux.pinMode(1, OUTPUT); // pin 1 = GPA1 of MCP2301X
  mux.pinMode(2, OUTPUT); // pin 2 = GPA2 of MCP2301X
  mux.pinMode(3, OUTPUT); // pin 3 = GPA3 of MCP2301X
  mux.pinMode(4, OUTPUT); // pin 4 = GPA4 of MCP2301X
  mux.pinMode(5, OUTPUT); // pin 5 = GPA5 of MCP2301X
  mux.pinMode(6, OUTPUT); // pin 6 = GPA6 of MCP2301X
  mux.pinMode(7, OUTPUT); // pin 7 = GPA7 of MCP2301X

  // set the last 8 pins as inputs with the built in pullup resistor
  // wire a momentary button for each of these pins on one end the other to ground
  mux.pinMode(8,  INPUT_PULLUP); // pin 8  = GPB0 of MCP2301X
  mux.pinMode(9,  INPUT_PULLUP); // pin 9  = GPB1 of MCP2301X
  mux.pinMode(10, INPUT_PULLUP); // pin 10 = GPB2 of MCP2301X
  mux.pinMode(11, INPUT_PULLUP); // pin 11 = GPB3 of MCP2301X
  mux.pinMode(12, INPUT_PULLUP); // pin 12 = GPB4 of MCP2301X
  mux.pinMode(13, INPUT_PULLUP); // pin 13 = GPB5 of MCP2301X
  mux.pinMode(14, INPUT_PULLUP); // pin 14 = GPB6 of MCP2301X
  mux.pinMode(15, INPUT_PULLUP); // pin 15 = GPB7 of MCP2301X
}
void loop(){
  // update the mux readings
  // the parameter passed is for a delay or wait in milliseconds, default: 1ms
  // this delay does NOT use the arduino delay() function, instead it uses
  // millis() so it is non-blocking, this delay is used to avoid reading the MCP
  // too often which would slow down your program.
  // you may set this value to 0 if you don't want any delay between readings.
  // all 16 pins are read at the same time, the state of inputs and outputs is
  // held in ram by the library so you can check what the last state of a pin was.
  mux.update(1);
  for(uint8_t i=0;i<8;i++){
    // turn each led on when you press the corresponding button, in otherwords
    // if you press the button connected to pin 8, the led in pin 0 will turn on
    // if you press the button connected to pin 9, the led in pin 1 will turn on
    // if you press the button connected to pin 10, the led in pin 2 will turn on
    // and so on
    mux.digitalWrite(i, !mux.digitalRead(i+8));
  }
}
```

### RoxButton
```c++
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
}

void loop(){
  // 3 parameters passed to update method

  // #1 the actual state of the button, in this case we passs the output of digitalRead
  //    RoxButton doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxButton is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxButton and the class will do the job for you.
  //    ROXBUTTON WILL DEBOUNCE THE READING, DO NO DEBOUNCE IT YOURSELF.

  // #2 the debounce time, this value is in milliseconds, default 50ms
  //    this is how long to wait before reading the button again

  // #3 the state of the button when it's active, if your pin has a pullup resisitor
  //    then the active state will be LOW, if it's a pulldown resistor then the
  //    active state is HIGH.
  button.update(digitalRead(0), 50, LOW);
  // the .held() function can take the time in milliseconds that you want to hold
  // the button for, if no value is passed the default 500ms will be used
  if(button.held()){
    Serial.println("* Button has been held for 500 milliseconds");
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
```

### RoxEncoder
```c++
#include <RoxMux.h>

RoxEncoder encoder;

void setup(){
  // for this example pin 0 and 1 on my teensy are connected to an encoder
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxEncoder Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  encoder.begin();
}

void loop(){
  // 3 parameters passed to update method

  // #1 the actual state of the encoder Pin A, in this case we passs the output of digitalRead
  //    RoxEncoder doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxEncoder is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxEncoder and the class will do the job for you.
  //    ROXENCODER WILL DEBOUNCE THE READING, DO NO DEBOUNCE IT YOURSELF.

  // #2 same as #1 but for Pin B of the encoder

  // #3 the debounce time, this value is in milliseconds, default 1ms
  //    this is how long to wait before reading the encoder again
  //    this is needed to clean some of the noise on the encoders.
  //    this value is 8-bits so keep it between 0 and 255

  // #4 the state of the encoder pins when they're active,
  //    if your pin has a pullup resisitor then the active state will be LOW,
  //    if it's a pulldown resistor then the active state is HIGH.
  encoder.update(digitalRead(0), digitalRead(1), 2, LOW);

  // the update method will read the encoder and if can be used to test if
  // there was movement, however you can also use .read() after the update
  // to check if there was movement

  // .read() checks if the encoder was rotated
  if(encoder.read()){
    // encoder has been rotated
    Serial.print("Encoder rotated ");
    // .increased() tells us if the encoder was rotated clockwise
    // you can also use .clockwise()
    // if this method returns false the encoder was rotated counter-clockwise
    if(encoder.increased()){
      Serial.println("Clockwise");
    } else {
      Serial.println("Counter-Clockwise");
    }
  }
}
```

### RoxPot
```c++
#include <RoxMux.h>

RoxPot pot;

void setup(){
  // for this example pin 0 and 1 on my teensy are connected to an pot
  pinMode(23, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxPot Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  pot.begin();
}

void loop(){
  // 2 parameters passed to update method

  // #1 the actual reading of the pot, in this case we passs the output of analogRead
  //    RoxPot doesn't read the pin itself so you have to pass the state of
  //    the pin, this is because RoxPot is meant to also work with Multiplexer
  //    outputs, so you have to read your pin or mux pin and then give that
  //    state to RoxPot and the class will do the job for you.

  // #2 (optional) debounce time in milliseconds, default: 1ms
  //    this value is used to debounce the pot, the potentiometer may have some
  //    noise to it and this can help remove some of it, add a 0.1uF capacitor
  //    between the pin and ground to remove additional noise
  //    Debouncing time is 8-bits so keep value at 0 to 255ms

  //    ROXPOT WILL LOWER THE RESOLUTION OF THE READING TO 7-BITS!!!
  if(pot.update(analogRead(23), 1)){
    uint8_t newPotReading = pot.read();
    Serial.print("Pot value: ");
    Serial.println(newPotReading);
  }
}
```
