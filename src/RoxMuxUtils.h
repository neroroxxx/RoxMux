/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to control one or more 74HC595 multiplexers

  This library will control each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/
#ifndef RoxMuxUtils_h
#define RoxMuxUtils_h

template <typename T>
class RoxMuxFlags {
  private:
    T flags;
  public:
    RoxMuxFlags(){
      reset();
    }
    bool read(uint8_t bit){
      return bitRead(flags,bit);
    }
    void write(uint8_t bit, bool value){
      bitWrite(flags, bit, value);
    }
    void on(uint8_t bit){
      bitWrite(flags, bit, 1);
    }
    void off(uint8_t bit){
      bitWrite(flags, bit, 0);
    }
    bool toggleIfTrue(uint8_t bit){
      if(bitRead(flags, bit)){
        bitClear(flags, bit);
        return true;
      }
      return false;
    }
    void reset(){
      flags = 0;
    }
};
#endif
