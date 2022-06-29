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
    uint16_t lastValue = 0;
    unsigned long lockout = 0;
    uint16_t resolution = 1023;
    uint8_t precision = 0;
  public:
    RoxPot(){

    }
    void begin(int16_t initialValue=0, uint8_t _resolution=7, uint8_t t_precision=2){
      reset(initialValue);
      precision = t_precision;
      // set how many bits to return
      // higher bit count may show random readings while the potentiometer
      // is static, this is due to noise.
      // lower bit count will generate a more stable reading but lowers the
      // maximum reading value.
      switch(_resolution){
        case 10: resolution = 1023; break;
        case 9:  resolution = 511; break;
        case 8:  resolution = 255; break;
        case 7:  resolution = 127; break;
        default: resolution = 1023; break;
      }
    }
    bool update(uint16_t reading, uint8_t lockoutHold=1, uint16_t calMin=0, uint16_t calMax=1023){
      if((millis()-lockout) < lockoutHold){
        return false;
      }
      calMax = calMax > resolution ? resolution : calMax;
      if(resolution<1023){
        // if the resolution is lower than 10 bits we have to map the current
        // reading from the typical 10-bit resolution to the new resolution
        // before mapping for the calibration values
        reading = map(reading, 0, 1023, 0, resolution);
      }
      reading = map(reading, calMin, calMax, 0, resolution);

      if(lastValue!=reading){
        if(
          (reading > (lastValue+precision) || reading < (lastValue-precision))
          ||
          ((lastValue!=calMin && reading==calMin) || (lastValue!=calMax && reading==calMax))
        ){
          lastValue = reading;
          lockoutHold = millis();
          return true;
        }
      }
      lockoutHold = millis();
      return false;
    }

    uint16_t read(){
      return lastValue;
    }
    void reset(uint16_t t_value=0){
      lastValue = t_value;
      lockout = millis();
    }
};


#endif
