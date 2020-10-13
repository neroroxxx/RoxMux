/*
  RoxDebug is a utility for printing multiple variables into 1 line of the Serial Monitor

  The best way to explain it's use is by example:

  uint8_t x = 0;
  uint8_t y = 1;
  uint8_t z = 2;

  Serial.print("x = ");
  Serial.print(x);
  Serial.print(" y = ");
  Serial.print(y);
  Serial.print(" z = ");
  Serial.print(z);

  The above will print out "x = 0 y = 1 z = 2"

  Instead you can use one line:

  ROX_PRINTLN("x = ", x, "y = ", y, "z = ", z);

  Note how there's no need for spaces in the Strings as every variable has
  a spaced added after it.

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  You must define ROX_DEBUG before you include the RoxMux library
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  Happy Coding!
*/

// when you are done debugging you can just remove the line "#define ROX_DEBUG"
// or comment it, you don't have to remove all the ROX_PRINTLN lines.
// if ROX_DEBUG is not define the Serial print code will not be compiled!
#define ROX_DEBUG
#include <RoxMux.h>

void setup(){
  Serial.begin(115200);
  delay(100);

  // print the value of x, y and z as it changes
  uint8_t x = 0;
  uint8_t y = 10;
  uint8_t z = 100;
  for(uint8_t i=0;i<10;i++){
    ROX_PRINTLN("x, y, z = ", x, y, z);
    x++;
    y++;
    z++;
    delay(500);
  }
}

void loop(){

}
