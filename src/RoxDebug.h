/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  NOTE: in order to use this macros you MUST add #define ROX_DEBUG before you
  include the RoxMux Library.

  Like this:

  #define ROX_DEBUG
  #include <RoxMux.h>

  Simple macro to print multiple variables into a single line in the Serial Monitor

  This is useful if you want to do something like this

  uint8_t myValue = 5;
  uint8_t myValue2 = 10;
  Serial.print("myValue is: ");
  Serial.print(myValue);
  Serial.print(" and myValue2 is: ");
  Serial.println(myValue2);

  The example above can be replaced with:

  uint8_t myValue = 5;
  uint8_t myValue2 = 10;
  ROX_PRINTLN("myValue is:", myValue, "and myValue2 is: ", myValue2);

  Both these methods will print out:

  "myValue is: 5 and myValue2 is: 10"

  Note how there's no need for spaces " " within the strings since the macro
  automatically adds a space between each variable.

  An upside of using ROX_DEBUG is that you can add debug code to your sketch
  and when you are done debugging you just have to commend #define ROX_DEBUG
  If ROX_DEBUG is not defined then none of the Serial.print code will be compiled.

  Use at your own risk.
*/
#ifndef ROX_DEBUG_h



#ifdef ROX_DEBUG

// use ROX_PRINTLN
// example ROX_PRINTLN(variable1, "hello", "world");

#define GET_ROXP_MACRO(_10,_9,_8,_7,_6,_5,_4,_3,_2,_1,NAME,...) NAME
#define ROX_DEBUG_PRINT(...) GET_ROXP_MACRO(__VA_ARGS__,ROXP10,ROXP9,ROXP8,ROXP7,ROXP6,ROXP5,ROXP4,ROXP3,ROXP2,ROXP1)(__VA_ARGS__)
#define ROXP1(s1) Serial.print(s1); Serial.print(" ");
#define ROXP2(s1,s2) ROXP1(s1) ROXP1(s2)
#define ROXP3(s1,s2,s3) ROXP2(s1,s2) ROXP1(s3)
#define ROXP4(s1,s2,s3,s4) ROXP2(s1,s2) ROXP2(s3,s4)
#define ROXP5(s1,s2,s3,s4,s5) ROXP4(s1,s2,s3,s4) ROXP1(s5)
#define ROXP6(s1,s2,s3,s4,s5,s6) ROXP5(s1,s2,s3,s4,s5) ROXP1(s6)
#define ROXP7(s1,s2,s3,s4,s5,s6,s7) ROXP6(s1,s2,s3,s4,s5,s6) ROXP1(s7)
#define ROXP8(s1,s2,s3,s4,s5,s6,s7,s8) ROXP7(s1,s2,s3,s4,s5,s6,s7) ROXP1(s8)
#define ROXP9(s1,s2,s3,s4,s5,s6,s7,s8,s9) ROXP8(s1,s2,s3,s4,s5,s6,s7,s8) ROXP1(s9)
#define ROXP10(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10) ROXP9(s1,s2,s3,s4,s5,s6,s7,s8,s9) ROXP1(s10)


#define GET_ROXPL_MACRO(_10,_9,_8,_7,_6,_5,_4,_3,_2,_1,NAME,...) NAME
#define ROX_DEBUG_PRINTLN(...) GET_ROXPL_MACRO(__VA_ARGS__,ROXPL10,ROXPL9,ROXPL8,ROXPL7,ROXPL6,ROXPL5, ROXPL4, ROXPL3, ROXPL2, ROXPL1)(__VA_ARGS__)
#define ROXPL1(s1) Serial.println(s1);
#define ROXPL2(s1,s2) Serial.print(s1); Serial.print(" "); ROXPL1(s2)
#define ROXPL3(s1,s2,s3) ROX_DEBUG_PRINT(s1, s2) ROXPL1(s3)
#define ROXPL4(s1,s2,s3,s4) ROX_DEBUG_PRINT(s1,s2,s3) ROXPL1(s4)
#define ROXPL5(s1,s2,s3,s4,s5) ROX_DEBUG_PRINT(s1,s2,s3,s4) ROXPL1(s5)
#define ROXPL6(s1,s2,s3,s4,s5,s6) ROX_DEBUG_PRINT(s1,s2,s3,s4,s5) ROXPL1(s6)
#define ROXPL7(s1,s2,s3,s4,s5,s6,s7) ROX_DEBUG_PRINT(s1,s2,s3,s4,s5,s6) ROXPL1(s7)
#define ROXPL8(s1,s2,s3,s4,s5,s6,s7,s8) ROX_DEBUG_PRINT(s1,s2,s3,s4,s5,s6,s7) ROXPL1(s8)
#define ROXPL9(s1,s2,s3,s4,s5,s6,s7,s8,s9) ROX_DEBUG_PRINT(s1,s2,s3,s4,s5,s6,s7,s8) ROXPL1(s9)
#define ROXPL10(s1,s2,s3,s4,s5,s6,s7,s8,s9,s10) ROX_DEBUG_PRINT(s1,s2,s3,s4,s5,s6,s7,s8,s9) ROXPL1(s10)

#define ROX_PRINTLN(...) ROX_DEBUG_PRINTLN(__VA_ARGS__)

#else

#define ROX_PRINTLN(...)

#endif



#endif
