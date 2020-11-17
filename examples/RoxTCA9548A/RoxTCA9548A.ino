/*
  Example for the RoxTCA9548A library

  The TCA9548A is an i2c multiplexer, it's used when you have multiple modules
  all with the same i2c address or when the module has limited address selection.

  Some i2c devices have a single fixed address or 2 addresses to select from,
  like in the case of some OLED displays, if you want to have more modules
  separately then you can use an i2c multiplexer like the TCA9548A.

  Many modules will have a 3 bit address giving you up to 8 independent devices
  like the MCP23017, using a multiplexer can increase the number of these devices.
  The MCP23017 gives you 16 IO pins, if you have 8 of them then you'd have 128 IO Pins,
  but by using just one TCA9548A you can have a total 1,024 IO pins! if you use
  use 8 TCA9548A's then you can have a theoretical 8,192 IO pins!


  Happy Coding!
*/

#include <RoxMux.h>

// the number "3" specifies how many TCA9548As are to be used
RoxTCA9548A <3> mux;

void setup(){
  // begin the wire library, the argument in the begin method specifies the clock speed
  // true = 400,000 bit/s
  // false = 100,000 bit/s
  mux.begin(true);
  // set the address of each TCA9548A
  // the first argument is the index of the TCA9548A and the second argument
  // is the address
  mux.setAddress(0, 0x70);
  // second
  mux.setAddress(1, 0x71);
  // third
  mux.setAddress(2, 0x72);

  // add a short delay
  delay(100);

  // When using more than one TCA9548A the library will add up the total ports
  // for example, since each TCA9548A has 8 ports if you use 3 TCA9548As
  // then you have a total of 8*3 or 24 ports so when selecting a port you have
  // to specify the port index based on the total count.
  // This is to make it easier to select ports

  // select i2c port 0 of first TCA9548A
  mux.select(0);
  // do something on port 0

  // select i2c port 3 of first TCA9548A
  mux.select(3);
  // do something on port 3

  // select i2c port 0 of second TCA9548A
  mux.select(8);
  // do something on port 0

  // select i2c port 4 of second TCA9548A
  mux.select(12);
  // do something on port 4
}

void loop(){

}
