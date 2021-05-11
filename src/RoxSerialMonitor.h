/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
#ifndef RoxSerialMonitor_H
#define RoxSerialMonitor_H

#include <Arduino.h>

class RoxSerialMonitor {
public:
  RoxSerialMonitor(){

  }
  void begin(uint16_t t_baud=0){
    if(t_baud==0){
      return;
    }
    Serial.begin(t_baud);
  }
  bool read(char *buff, uint16_t len){
    static uint8_t i = 0;
    char receivedChar;
    while(Serial.available() > 0 && !filled){
      receivedChar = Serial.read();
      if((uint8_t)receivedChar != 10) {
        buff[i] = receivedChar;
        i++;
        if(i >= len){
          i = len-1;
        }
      } else {
        buff[i] = 0;
        i = 0;
        filled = true;
      }
    }
    if(filled){
      filled = false;
      return true;
    }
    return false;
  }
private:
  bool filled = false;
};
#endif
