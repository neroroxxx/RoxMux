/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This Library is meant to work along with the Rox74HC595,
  This class can control a set of LEDs to turn them into a VU Meter style.

  Use at your own risk.
*/
#ifndef RoxLedMeter_h
#define RoxLedMeter_h

#include "RoxFlags.h"

// _total, how many leds will be used for the VU meter
// _fromPin the first LED used
// _low, the lowest reference value, ex: if your device is generating a value from 0 to 15, then this should be 0
// _high, the highest reference value, ex: if your device is generating a value from 0 to 15, then this should be 15
template <uint16_t _total, uint16_t _fromPin, int16_t _low, int16_t _high>
class RoxLedMeter {
private:
  int16_t lastValue = _low;
  void (*onChangeCallback)(uint16_t n, bool state);
  void set(uint16_t value){
    // value will now hold how many leds should be on
    value = map(value, _low, _high, 0, _total);
    if(lastValue != value){
      // value has changed
      if(value > lastValue){
        // new value is higher than the last one
        // in this case we'll only turn on the ones that weren't on before
        for(uint16_t i=0, n=(value-lastValue), e=(_fromPin+lastValue) ; i < n ; i++){
          onChangeCallback(e+i, HIGH);
        }
      } else {
        if(value==0){
          for(uint16_t i=0, e=_fromPin ; i < lastValue ; i++){
            onChangeCallback(e+i, LOW);
          }
        } else {
          for(uint16_t i=(_fromPin+value), n=(_fromPin+lastValue) ; i < n ; i++){
            onChangeCallback(i, LOW);
          }
        }
      }
      lastValue = value;
    }
  }
public:
  RoxLedMeter(){
    onChangeCallback = 0;
  }
  void onUpdate(void (*fptr)(uint16_t n, bool state)){
    onChangeCallback = fptr;
  }
  void setValue(uint16_t value){
    if(onChangeCallback){
      set(value);
    }
  }

};
#endif
