/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the encoder utility of BMC
  https://github.com/neroroxxx/BMC
*/
#ifndef RoxEncoder_h
#define RoxEncoder_h

#define ROX_ENCODER_FLAG_ACTIVITY 0
#define ROX_ENCODER_FLAG_INCREASED 1

class RoxEncoder {
  private:
    RoxFlags <uint8_t> flags;
    int8_t lastState;
    int8_t newState;
    unsigned long debounce;
    const int8_t lookupTable[16] = {
       0,-1,  1,  0,
       1, 0,  0, -1,
      -1, 0,  0,  1,
       0, 1, -1,  0
    };
  public:
    RoxEncoder(){
      flags.reset();
    }
    void begin(){
      debounce = millis();
    }
    bool update(bool pinA, bool pinB, uint8_t debounceTime=1, bool activeState=LOW){
      flags.reset();
      if((unsigned long)millis()-debounce < debounceTime){
        return false;
      }
      debounce = millis();
      if(activeState==LOW){
        pinA = !pinA;
        pinB = !pinB;
      }
      int8_t output = 0;
      uint8_t now = (pinA << 1) | pinB;
      if(lastState != now){
        newState += lookupTable[now | (lastState<<2)];
        if(now == 3){
          output = (newState >> 2) + (lastState>>1);
          newState = 0;
          flags.on(ROX_ENCODER_FLAG_ACTIVITY);
          flags.write(ROX_ENCODER_FLAG_INCREASED, (output>0));
          lastState = now;
          return true;
        }
        lastState = now;
      }
      return false;
    }
    bool read(){
      return flags.read(ROX_ENCODER_FLAG_ACTIVITY);
    }
    bool increased(){
      return flags.read(ROX_ENCODER_FLAG_INCREASED);
    }
    bool decreased(){
      return !increased();
    }
    bool clockwise(){
      return increased();
    }
    bool counterClockwise(){
      return !increased();
    }
};

#endif
