/*
  !!!!!!!!!!!!!! NOTICE !!!!!!!!!!!!!!
  RoxPot has be redone for higher resolution is better accuracy,
  The begin method that allows you change pass the following parameters

  @initialValue: allows to pass the first reading to compare to

  @bitResolution: how many bits the return value will be, you can set it to:
                  10 for 10-bit readings, 0 to 1023
                  9 for 9-bit readings, 0 to 511
                  8 for 8-bit readings, 0 to 255
                  7 for 7-bit readings, 0 to 127
                  The lower the resolution the more stable the reading.

  @precision: this value will make cleaner readings, normally a pot may give
              random readings while the pot is static, you may see a value
              that only changes from 10 to 11 then back to 10 and 11 again
              again and again, etc.
              With this setting the pot must change values at least by the
              value of @precision, the default is 3 which works great.
              Keep in mind, the higher the @bitResolution the lower this value
              can be for clean readings, for MIDI you can use the default
              @bitResoltion = 7
              @precision = 2
              this will give you a 7-bit reading from 0 to 127 perfect for MIDI

  These 3 parameters are optional but will default to 0, 7, 2 respectibly
  This is great for MIDI which is what I focus on.

  !!!!!!!!!!!!!! NOTICE !!!!!!!!!!!!!!


  RoxPot reads potentiometers but allows you to set a custom resolution

  RoxPot is different from must pot reading libraries because it's meant
  to be used with either the Pin on your arduino or a value read from a multiplexer

  Because of this you do not have to tell RoxPot what pin the pot is connected
  to, instead you give RoxPot the direct reading of your pot, in other words, you
  read the pin value and RoxPot handles debouncing and determining the state.

  Because of this you must add the pinMode for your pin in your setup code

*/
#include <RoxMux.h>

// this value can range from 10 to 7
// this value determines how many bits to map down to.
// 10 bits will have a maximum return of 1023
// the lower the resoltution, the more stable the reading will be
#define READING_BIT_RESOLUTION 10
// this value will determine the precision of the reading,
// the higher this value the less "random" readings you will get
#define READING_ERROR_PRESICION 3
// this value is to debounce the readings, it represents how many milliseconds
// to wait before returning a value reading.
#define POT_LOCKOUT_TIME 50

RoxPot pot;

void setup(){
  pinMode(23, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxPot Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  // you can add an initial value as a parameter as a way to trigger the initial reading
  pot.begin(0, READING_BIT_RESOLUTION, READING_ERROR_PRESICION);
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

  // You can add 2 more parameters to the .update method, a calibration min and max
  // this is if you know the lowest and highest reading your pot can produce
  if(pot.update(analogRead(23), POT_LOCKOUT_TIME)){
    Serial.print("Pot value: ");
    Serial.println(pot.read());
  }
}
