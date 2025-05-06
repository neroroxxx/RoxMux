/*
  https://www.roxxxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the encoder utility of BMC
  https://github.com/neroroxxx/BMC
*/
#ifndef RoxEncoderWithButton_h
#define RoxEncoderWithButton_h

#define ROX_ENCODER_FLAG_ACTIVITY 0
#define ROX_ENCODER_FLAG_CLOCKWISE 1
#define ROX_ENCODER_FLAG_LAST_TURN_DIRECTION 2

template <uint8_t buttonDebounce=50, bool buttonActive=LOW, uint8_t encoderDebounce=1, bool encActive=LOW>
class RoxEncoderWithButton {
  private:
    RoxButton button;
    RoxEncoder encoder;
    void (*onUpdateCallback)(uint8_t btnType, uint8_t t_ticks, bool t_clockwise);
  public:
    RoxEncoderWithButton(){
      onUpdateCallback = 0;
    }
    void begin(void (*fptr)(uint8_t btnType, uint8_t t_ticks, bool t_clockwise)){
      onUpdateCallback = fptr;
      button.begin();
      encoder.begin();
    }
    void setTickSensitivity(uint8_t t_value){
      encoder.setTickSensitivity(t_value);
    }
    void setDoublePressThreshold(uint16_t t_thres){
      button.setDoublePressThreshold(t_thres);
    }
    void update(bool a, bool b, bool sw, uint16_t t_hold=ROX_BTN_HOLD_THRESH, bool ignoreAfterHold=false){
      encoder.update(a, b, encoderDebounce, encActive);
      button.update(sw, buttonDebounce, buttonActive);

      uint8_t btnType = ROX_NONE;
      if(button.held(t_hold)){
        btnType = ROX_HELD;
      } else if(button.doublePressed()){
        btnType = ROX_DOUBLE;
      } else if(button.pressed()){
        btnType = ROX_PRESSED;
      } else if(button.released(ignoreAfterHold)){
        btnType = ROX_RELEASED;
      }
      if(onUpdateCallback && (encoder.getTicks()>0 || btnType!=ROX_NONE)){
        onUpdateCallback(btnType, encoder.getTicks(), encoder.clockwise());
      }
    }
};

#endif
