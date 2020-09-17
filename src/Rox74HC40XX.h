/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to read one or more 74HC4067 or 74HC4051 analog-in multiplexers

  Rox74HC4067, this is a 16 pin analog-in multiplexer
  Rox74HC4051, this is a 8 pin analog-in multiplexer

  This library will read each pin of the mux and store it in RAM

  It was designed and tested for PJRC Teensy boards only.

  Use at your own risk.
*/

#ifndef Rox74HC40XX_h
#define Rox74HC40XX_h

#include <Arduino.h>

template <uint8_t _muxCount, uint8_t _muxChannels, uint8_t _muxPins>
class Rox74HC40XX {
  public:
    Rox74HC40XX(){}
    void begin(uint8_t ch1, uint8_t ch2, uint8_t ch3, int8_t ch4=-1){
      if(_muxChannels<3 || _muxChannels>4){
        delay(1000);
        Serial.println("channel must be 3 or 4");
        while(1);
      }
      if(_muxChannels==3 && _muxPins!=8){
        delay(1000);
        Serial.println("_muxPins must be 8");
        while(1);
      } else if(_muxChannels==4 && _muxPins!=16){
        delay(1000);
        Serial.println("_muxPins must be 16");
        while(1);
      }
      channels[0] = ch1;
      channels[1] = ch2;
      channels[2] = ch3;
      if(ch4!=-1 && _muxChannels==4){
        channels[3] = ch4;
      }
      for(uint8_t i = 0 ; i < _muxChannels ; i++){
        pinMode(channels[i], OUTPUT);
      }
      delay(10);
      timeout = millis();
    }
    // @n: the index of the mux
    // @pin: the pin on the teensy connected to that mux' signal pin
    // this method must be called for each mux
    void setSignalPin(uint8_t n, uint8_t pin){
      if(n < _muxCount){
        signalPin[n] = pin;
        pinMode(pin, INPUT);
      }
    }
    // these analog multiplexers require a small delay between pin read
    // so that their capacitor for each pin is charged/discharged
    // since we don't want to use an actual delay() we use an elapsedMillis
    // you can pass a value to this function in your sketch to change the
    // number of milliseconds between each pin read.
    void update(uint8_t readInterval=1){
      if(millis()-timeout >= readInterval){
        readMux();
        timeout = millis();
      }
    }
    uint16_t read(uint16_t n){
      if(n < totalPins){
        return values[n];
      }
      return 0;
    }
private:
    uint8_t currentChannel = 0;
    uint8_t channels[_muxChannels];
    uint8_t signalPin[_muxCount];
    uint16_t values[_muxCount*_muxPins];
    unsigned long timeout;
    const uint16_t totalPins = (_muxCount*_muxPins);

    void readMux(){
      for(uint8_t i = 0 ; i < _muxCount ; i++){
        uint8_t index = (i*_muxPins) + currentChannel;
        if(index < totalPins){
          values[index] = analogRead(signalPin[i]);
        }
      }
      // go to the next channel
      currentChannel++;
      if(currentChannel >= _muxPins){
        currentChannel = 0;
      }
      // set the channel pins
      for(uint8_t i=0 ; i < _muxChannels ; i++){
        digitalWrite(channels[i], bitRead(currentChannel, i));
      }
    }
};

template <uint8_t _muxCount>
class Rox74HC4067 : public Rox74HC40XX <_muxCount, 4, 16> {};

template <uint8_t _muxCount>
class Rox74HC4051 : public Rox74HC40XX <_muxCount, 3, 8> {};


#endif
