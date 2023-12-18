/*
  RoxLed lets you easily control an led without constantly doing a digitalWrite
  With RoxLed you can turn the led on/off but in addition you can blink the led
  (the led will turn on and off until you turn it off) and pulse it (the led will
  turn on for a predetermined time then to turn off and stay off)
  You can use RoxLed to directly control a pin or within Rox74HC595 and RoxMCP2301X
*/
#define ROX_DEBUG
#include <RoxMux.h>

// for this example we'll use the pin 3 which is a PWM pin on teensy
RoxFadingLed led;

void setup(){
  Serial.begin(115200);
  Serial.println("RoxFadingLed Example");
  // little delay before starting
  delay(100);

  // begin() will take in the pin number
  // for this example we'll use the pin 3 which is a PWM pin on teensy
  led.begin(3);

  // the led fading rate is handled in Beats per Minute
  // use .setBPM with a value between 30 and 300
  // the lower the value the slower the fading rate
  led.setBPM(120);

  // turn the led on
  led.on();
}

void loop(){
  // use led.on(), led.off() or led.toggle() to control the led
  // the led will transition it's brightness from 0 to 255 
  // then back down to 0 by 1 value.
  led.update();
}
