/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.


  Use at your own risk.
*/
#ifndef RoxTCA9548A_h
#define RoxTCA9548A_h

#include <Wire.h>

// pin mode
#define ROX_TCA9548A_DEFAULT_ADDR 0x70

class _RoxTCA9548A {
public:
  _RoxTCA9548A(){}

};

template <uint8_t muxCount>
class RoxTCA9548A {
private:
  uint8_t muxAddress[muxCount];
  void transmit(uint8_t t_addr, uint8_t t_mask){
    Wire.beginTransmission(t_addr);
    Wire.write(t_mask);
    Wire.endTransmission();
  }
public:
  RoxTCA9548A(){
    memset(muxAddress, ROX_TCA9548A_DEFAULT_ADDR, muxCount);
  }
  void begin(bool fast=false){
    Wire.begin();
    Wire.setClock((fast) ? 400000 : 100000);
  }
  void setAddress(uint8_t index, uint8_t address){
    if(index < muxCount && (address>=0x70 && address<=0x77)){
      muxAddress[index] = address;
    }
  }
  void select(uint8_t port){
    if(port < (muxCount*8)){
      if(muxCount > 1){
        uint8_t x = port/8;
        uint8_t address = muxAddress[x];
        uint8_t mask = 1 << (port - (x*8));
        transmit(address, mask);
      } else {
        transmit(muxAddress[0], (1 << port));
      }
    }
  }

};


#endif
