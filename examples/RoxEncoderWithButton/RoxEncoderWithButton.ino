/*
  This class uses encoder and button objects to make it easier to read
  both together, this class only works with callbacks.
  It's meant to help when using the typical encoders that have a switch pin.
  The class is a template so needs a few values
*/
#include <RoxMux.h>


// the template parameters are:
// button debounce time
// encoder active logic
// encoder debounce time
// button active logic
RoxEncoderWithButton <50, LOW, 2, LOW> encoder;

void setup(){
  // this example uses pins 0 and 1 on the encoder and pin 2 on the switch
  // also uses the built-in pullup resistor of the pins
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  // start the serial monitor and print a line
  Serial.begin(115200);
  // little delay before starting
  delay(100);
  Serial.println("RoxEncoderWithButton Example");
  // always add the .begin() method on the setup();
  encoder.begin(onUpdate);
}
void loop(){
  // @1 pass the reading of the encoder's pin A
  // @2 pass the reading of the encoder's pin B
  // @3 pass the reading of the button's pin
  // @4 how long the button should be pressed for before triggering a HELD (in ms)
  // @5 true if you want to ignore the "released" reading after "held" was triggered
  encoder.update(digitalRead(0), digitalRead(1), digitalRead(2), 500, false);
  // see the RoxButton example for more info on the setDoublePressThreshold() method.
  // encoder.setDoublePressThreshold(300);
}
void onUpdate(uint8_t btnType, uint8_t ticks, bool clockwise){
  // callback passes values from both the button and encoder
  // btnType may be any of the following
  // ROX_NONE      : the button hasn't change state
  // ROX_RELEASED  : the button was initially pressed
  // ROX_PRESSED   : the button was released
  // ROX_HELD      : the button was held by the specified hold time
  if(btnType!=ROX_NONE){
    Serial.print("Button ");
    if(btnType==ROX_RELEASED){
      Serial.print(" RELEASED");
    } else if(btnType==ROX_PRESSED){
      Serial.print(" PRESSED");
    } else if(btnType==ROX_HELD){
      Serial.print(" HELD");
    }
  }
  // if ticks is not 0, the encoder has been rotated
  if(ticks>0){
    Serial.print("encoder rotated ");
    Serial.print(clockwise?"Clockwise":"Counter-Clockwise");
    Serial.print(", ticks");
    Serial.println(ticks);
  }
}
