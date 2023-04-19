/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the encoder utility of BMC
  https://github.com/neroroxxx/BMC
*/
#ifndef RoxOctoswitch_h
#define RoxOctoswitch_h

#define ROX_ENCODER_FLAG_ACTIVITY 0
#define ROX_ENCODER_FLAG_CLOCKWISE 1
#define ROX_ENCODER_FLAG_LAST_TURN_DIRECTION 2

template <uint8_t _octoCount, uint8_t buttonDebounce=50>
class RoxOctoswitch {
  private:
    Rox74HC165 <_octoCount> mux;
    RoxButton buttons[8*_octoCount];
    uint8_t ignoreAfterHold[_octoCount];
    void (*onUpdateCallback)(uint16_t btnIndex, uint8_t btnType);
    bool getIgnoreAfterHold(uint16_t n){
      if(n < 8){
        return bitRead(ignoreAfterHold[0], n);
      }
      return bitRead(ignoreAfterHold[(n>>3)], (n&0x07));
    }
  public:
    RoxOctoswitch(){
      onUpdateCallback = 0;
      memset(ignoreAfterHold, 0, sizeof(ignoreAfterHold[0])*_octoCount);
    }
    void begin(uint8_t t_pinData, uint8_t t_pinLoad, uint8_t t_pinClk){
      mux.begin(t_pinData, t_pinLoad, t_pinClk);
      for(uint16_t i=0, n=(8*_octoCount) ; i < n; i++){
        buttons[i].begin();
      }
    }
    void setCallback(void (*fptr)(uint16_t btnIndex, uint8_t btnType)){
      onUpdateCallback = fptr;
    }
    void setDoublePressThreshold(uint16_t n, uint16_t t_thres){
      if(n < (8*_octoCount)){
        buttons[n].setDoublePressThreshold(t_thres);
      }
    }
    void setIgnoreAfterHold(uint16_t n, bool value){
      if(n < (8*_octoCount)){
        bitWrite(ignoreAfterHold[(n>>3)], (n&0x07), value);
      }
    }
    void update(uint16_t t_hold=ROX_BTN_HOLD_THRESH){
      mux.update();
      for(uint16_t i=0, n=(8*_octoCount) ; i < n; i++){
        buttons[i].update(mux.read(i), buttonDebounce, LOW);
        uint8_t btnType = ROX_NONE;
        if(buttons[i].held(t_hold)){
          btnType = ROX_HELD;
        } else if(buttons[i].doublePressed()){
          btnType = ROX_DOUBLE;
        } else if(buttons[i].pressed()){
          btnType = ROX_PRESSED;
        } else if(buttons[i].released(getIgnoreAfterHold(i))){
          btnType = ROX_RELEASED;
        }
        if(onUpdateCallback && btnType!=ROX_NONE){
          onUpdateCallback(i, btnType);
        }
      }
    }
};

#endif
