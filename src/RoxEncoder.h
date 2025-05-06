/*
  https://www.roxxxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the encoder utility of BMC
  https://github.com/neroroxxx/BMC
*/
#ifndef RoxEncoder_h
#define RoxEncoder_h

#define ROX_ENCODER_FLAG_ACTIVITY 0
#define ROX_ENCODER_FLAG_CLOCKWISE 1
#define ROX_ENCODER_FLAG_LAST_TURN_DIRECTION 2

class RoxEncoder {
  private:
    RoxFlags <uint8_t> flags;
    int8_t lastState;
    int8_t newState;
    unsigned long debounce;
    uint16_t lastTurn = 0;
    uint8_t ticks = 0;
    uint8_t tickSensitivity = 2;
    const int8_t lookupTable[16] = {
       0,-1,  1,  0,
       1, 0,  0, -1,
      -1, 0,  0,  1,
       0, 1, -1,  0
    };
    void (*onUpdateCallback)(uint8_t t_ticks, bool clockwise);
    bool getLastDirection(){
      return flags.read(ROX_ENCODER_FLAG_LAST_TURN_DIRECTION);
    }
    void setLastDirection(bool value){
      flags.write(ROX_ENCODER_FLAG_LAST_TURN_DIRECTION, value);
    }
  public:
    RoxEncoder(){
      onUpdateCallback = 0;
      flags.reset();
    }
    void onUpdate(void (*fptr)(uint8_t t_ticks, bool clockwise)){
      onUpdateCallback = fptr;
    }
    void begin(){
      debounce = millis();
    }
    bool update(bool pinA, bool pinB, uint8_t debounceTime=1, bool activeState=LOW){
      ticks = 0;
      bool ld = getLastDirection();
      flags.reset();
      setLastDirection(ld);
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
          flags.write(ROX_ENCODER_FLAG_CLOCKWISE, (output > 0));
          uint16_t currentTurn = millis()&0xFFFF;
          uint16_t subs = (currentTurn-lastTurn);
          if(getLastDirection() == clockwise()){

            if(subs < 10 && tickSensitivity > 1){
              ticks = 3 * tickSensitivity;
            } else if(subs < 40){
              ticks = 2 * tickSensitivity;
            } else if(subs < 60){
              ticks = 1 * tickSensitivity;
            } else {
              ticks = 1;
            }
          }
          setLastDirection(clockwise());
          lastTurn = millis()&0xFFFF;
          lastState = now;
          // if the callback is used the extra ticks variable is reset so
          // the getTicks() method will always return 0
          if(onUpdateCallback){
            onUpdateCallback(clockwise(), getTicks());
            return false;
          }
          return true;
        }
        if((millis()&0xFFFF)-lastTurn>50){
          lastTurn = 0;
        }
        lastState = now;
      }
      return false;
    }
    void setTickSensitivity(uint8_t t_value){
      tickSensitivity = constrain(t_value, 1, 3);
    }
    uint8_t getTicks(){
      return ticks;
    }
    bool read(){
      return flags.read(ROX_ENCODER_FLAG_ACTIVITY);
    }
    bool clockwise(){
      return flags.read(ROX_ENCODER_FLAG_CLOCKWISE);
    }
    bool counterClockwise(){
      return !clockwise();
    }
    bool increased(){
      return clockwise();
    }
    bool decreased(){
      return !clockwise();
    }
};

#endif
