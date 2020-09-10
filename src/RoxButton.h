/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
#ifndef RoxButton_h
#define RoxButton_h

#define ROX_BTN_STATE                 0
#define ROX_BTN_STATE_CHANGED         1
#define ROX_BTN_STATE_HELD            2
#define ROX_BTN_STATE_HOLD_TRIGGERED  3

#define ROX_BTN_DEFAULT_HOLD_THRESH   500

#define ROX_NONE      0
#define ROX_RELEASED  1
#define ROX_PRESSED   2
#define ROX_HELD      3

class RoxButton {
  private:
    unsigned long debounce = 0;
    unsigned long holdDebounce = 0;
    RoxMuxFlags <uint8_t> flags;
    bool isPressed(){
      return flags.read(ROX_BTN_STATE);
    }
    bool stateChanged(){
      return flags.read(ROX_BTN_STATE_CHANGED);
    }
    bool btnHeld(){
      return flags.read(ROX_BTN_STATE_HELD);
    }
  public:
    RoxButton(){
      debounce = 0;
      holdDebounce = 0;
      flags.reset();
    }
    void begin(){
      debounce = millis();
    }
    bool update(bool state, uint16_t debounceTime=50, bool activeState=LOW){
      flags.off(ROX_BTN_STATE_CHANGED);
      bool pressed = (activeState==LOW) ? !state : state;
      if((millis() - debounce) >= debounceTime){
        debounce = millis();
        if(pressed != flags.read(ROX_BTN_STATE)){
          flags.write(ROX_BTN_STATE, pressed);
          flags.write(ROX_BTN_STATE_CHANGED, true);
          flags.write(ROX_BTN_STATE_HELD, pressed);
          holdDebounce = millis();
          return true;
        }
      }
      return false;
    }
    uint8_t read(uint16_t holdTime=ROX_BTN_DEFAULT_HOLD_THRESH){
      if(stateChanged()){
        if(held(holdTime)){
          return ROX_HELD;
        } else {
          if(isPressed()){
            return ROX_PRESSED;
          } else {
            return ROX_RELEASED;
          }
        }
      }
      return ROX_NONE;
    }
    bool pressed(){
      return stateChanged() && isPressed();
    }
    // you can pass a boolean to released() this will determine weather the
    // release will be triggered after a hold has been triggered
    bool released(bool ignoreAfterHold=false){
      bool state = stateChanged() && !isPressed();
      bool holdTriggered = flags.read(ROX_BTN_STATE_HOLD_TRIGGERED);
      if(state){
        flags.off(ROX_BTN_STATE_HOLD_TRIGGERED);
      }
      if(state && ignoreAfterHold && holdTriggered){
        return false;
      }
      return state;
    }
    bool held(uint16_t holdTime=ROX_BTN_DEFAULT_HOLD_THRESH){
      bool state = isPressed() && btnHeld() && ((millis()-holdDebounce)>=holdTime);
      if(state && !flags.read(ROX_BTN_STATE_HOLD_TRIGGERED)){
        flags.on(ROX_BTN_STATE_HOLD_TRIGGERED);
        return true;
      }
      return false;
    }
    bool getCurrentState(){
      return isPressed();
    }
};

#endif
