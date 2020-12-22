/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to control one or more 74HC595 multiplexers

  This library will control each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/
#ifndef RoxFlags_h
#define RoxFlags_h

template <typename T>
class RoxFlags {
  private:
    T flags;
  public:
    RoxFlags(){
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
    // toggle and return new value
    bool toggle(uint8_t bit){
      flags ^= 1UL << bit;
      return bitRead(flags, bit);
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
    void set(T t_flags){
      flags = t_flags;
    }
    T get(){
      return flags;
    }
};
#endif
