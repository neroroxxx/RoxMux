/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the pot utility of BMC
  https://github.com/neroroxxx/BMC

  RoxPot will lower the resolution of your pot to 7-bits, the output will
  be between 0 and 127
*/
#ifndef RoxPot_h
#define RoxPot_h

class RoxPot {
  private:
    RoxFlags <uint8_t> flags;
    uint8_t value = 0;
    uint8_t rawValue = 0;
    uint8_t counter = 0;
    unsigned long debounce = 0;
  public:
    RoxPot(){

    }
    void begin(){
      debounce = millis();
    }
    bool update(uint16_t reading, uint8_t debounceTime=1){
      if((millis()-debounce) < debounceTime){
        return false;
      }
      debounceTime = millis();
      reading = map(reading, 0, 1024, 0, 128);
      uint8_t lastValue = value;
      uint8_t lastRawValue = rawValue;
      rawValue = reading;
      if(rawValue != lastRawValue){
        counter = 0;
      }
      value = floor((value + (rawValue*counter)) / (counter+1));
      value = (abs(rawValue - value) > 10) ? ((value + rawValue)/2) : value;
      counter++;
      if(counter >= 255){
        counter = 0;
      }
      return (lastValue != value);
    }
    uint8_t read(){
      return value;
    }
};

#endif
