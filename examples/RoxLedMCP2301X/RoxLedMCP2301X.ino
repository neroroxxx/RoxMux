/*
  RoxLed lets you easily control an led without constantly doing a digitalWrite
  With RoxLed you can turn the led on/off but in addition you can blink the led
  (the led will turn on and off until you turn it off) and pulse it (the led will
  turn on for a predetermined time then to turn off and stay off)
  You can use RoxLed to directly control a pin or within Rox74HC595 and RoxMCP2301X
*/
#include <RoxMux.h>


RoxMCP23017 <0x20> mux;

// for this example we'll use the built-in led on pin 13
RoxLed led;

// used to change modes
unsigned long prevTime = 0;
uint8_t mode = 0;

// which pin of your MCP23017 or MCP23018 will be used
#define MCP_PIN 8

void setup(){
  Serial.begin(115200);
  Serial.println("RoxLedMCP2301X Example");
  // little delay before starting
  delay(100);

  // initialize the MCP23017
  mux.begin(true);
  // set pin as output
  mux.pinMode(MCP_PIN, OUTPUT);



  // begin() will take in the pin number, if you will use it with a mux then
  // leave the field blank.
  // we'll leave it blank since it's a mux pin
  led.begin();
  // you can set a mode for the led when it's on, there are 3 modes
  // default (led on or off), blink and pulse.
  // you can set the mode here and change this anywhere in your code

  // DEFAULT: in this mode when you call the .on() method the led
  // will turn on and stay on until you call the .off() method
  led.setMode(ROX_DEFAULT);

  // BLINK: in this mode when you call the .on() method the led
  // will turn on and off repeately until you call the .off() method
  // you can set the rate at which the led turns on/off in the .update() method
  //led.setMode(ROX_BLINK);

  // PULSE: this method is similar to BLINK but instead the led will light up
  // for 75 milliseconds, then turn off and repeat based on the specified rate.
  // For PULSE the rate parameter passed to the .update() method will instead
  // be used for the next time the led will pulse again.
  // for example the pulse method can be used for TEMPO, if you pass a value of
  // 500 to the .update() method then the led will turn on every 500 milliseconds
  // but only for 75 milliseconds at a time, where the BLINK method will
  // keep the led on for 500ms then off for 50ms and so on.
  //led.setMode(ROX_PULSE);

  prevTime = millis();

  // turn the led on
  led.on();
}


void ledControl(uint8_t pin, RoxLed &t_led, uint8_t rate=75){
  uint8_t cmd = t_led.update(rate);
  if(cmd==1){
    digitalWrite(pin, LOW);
  } else if(cmd==2){
    digitalWrite(pin, HIGH);
  }
}


void loop(){
  mux.update(1);

  // the .update() method takes in a rate, this rate is used for blink and pulse.
  // the .update() method will also return a byte from 0 to 2 where:
  // 0 - no change to the state of the led.
  // 1 - the led has been turned off (only returned once when the state changes)
  // 2 - the led has been turned on (only returned once when the state changes)

  // for this example tho we'll change between modes every 2 seconds
  if((unsigned long) millis()-prevTime >= 5000){
    if(mode>=2){
      mode = 0;
    } else {
      mode++;
    }
    led.setMode(mode);
    prevTime = millis();
  }

  // since we are using a mux, instead of controlling a pin from on the board
  // we are going to pass the state the led should be to the mux
  // we do this by passing the pin of the mux we'll use AND the actual RoxLed object
  // the .ledControl method is also used by Rox74HC595
  // it takes 3 parameters:
  // 1st the pin of the Mux that will be controlled
  // 2nd the RoxLed Object for that specific led
  // 3rd the rate to blink or to pulse (can be left blank for on off)
  if(mode==ROX_DEFAULT){
    //led.update();
    mux.ledControl(MCP_PIN, led);
  } else if(mode==ROX_BLINK){
    // turn led on/off for 50ms at a time
    //led.update(50);
    mux.ledControl(MCP_PIN, led, 50);
  } else if(mode==ROX_PULSE){
    // light up led every 500ms
    mux.ledControl(MCP_PIN, led, 500);
    //led.update(500);
  }
  // You can type anything in the serial monitor to toggle the led on/off
  // make sure you hit send and you specify the right baud rate in setup
  bool toggle = false;
  while(Serial.available() > 0){
    Serial.read();
    toggle = true;
  }
  if(toggle){
    bool state = led.toggle();
    Serial.print("toggled: ");
    Serial.println(state ? "ON" : "OFF");
  }
  // anytime you want to turn the led off just call this method
  // next time you call .on() the led will return to it's previous mode
  // led.off();
}
