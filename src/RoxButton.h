/*
  https://www.roxxxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a basic port from the button utility of BMC
  https://github.com/neroroxxx/BMC
*/
#ifndef RoxButton_h
#define RoxButton_h

#define ROX_BTN_STATE                 0
#define ROX_BTN_STATE_CHANGED         1
#define ROX_BTN_STATE_HELD            2
#define ROX_BTN_STATE_HOLD_TRIGGERED  3
#define ROX_BTN_STATE_DBL_ACTIVE      4
#define ROX_BTN_STATE_DBL_TRIGGERED   5
#define ROX_BTN_STATE_DBL_IGNORE_REL  6

#define ROX_BTN_HOLD_THRESH   500

#define ROX_NONE      0
#define ROX_RELEASED  1
#define ROX_PRESSED   2
#define ROX_HELD      3
#define ROX_DOUBLE    4

class RoxButton {
  private:
    unsigned long debounce = 0;
    unsigned long holdDebounce = 0;
    unsigned long dblDebounce = 0;
    uint16_t doublePressTime = 0;
    RoxFlags <uint8_t> flags;
    void (*onUpdateCallback)(uint8_t type);
    void onUpdate(void (*fptr)(uint8_t type)){
      onUpdateCallback = fptr;
    }
    bool isPressed(){
      return flags.read(ROX_BTN_STATE);
    }
    bool stateChanged(){
      return flags.read(ROX_BTN_STATE_CHANGED);
    }
    bool btnHeld(){
      return flags.read(ROX_BTN_STATE_HELD);
    }
    bool setDblPress(){
      if(flags.toggleIfTrue(ROX_BTN_STATE_DBL_ACTIVE)){
        // check if the button was pressed again before the threshold time has passed
        if(((unsigned long)millis()-dblDebounce) <= doublePressTime){
          // the time between presses fits within our specified time
          flags.on(ROX_BTN_STATE_DBL_TRIGGERED);
          return true;
        }
        // if the time is past the time above then we timedout
      }
      return false;
    }
    void setDblRelease(){
      if(!flags.read(ROX_BTN_STATE_DBL_ACTIVE)){
        // first press of button, button was released
        // at this point start the timer and set the flag that says we are
        // waiting for a the next press of thebutton
        flags.on(ROX_BTN_STATE_DBL_ACTIVE);
        dblDebounce = millis();
      }
    }
  public:
    RoxButton(){
      onUpdateCallback = 0;
      debounce = 0;
      holdDebounce = 0;
      flags.reset();
    }
    void begin(){
      debounce = millis();
    }
    bool update(bool state, uint16_t t_debounce=50, bool t_logic=LOW){
      flags.off(ROX_BTN_STATE_CHANGED);
      bool pressed = (t_logic==LOW) ? !state : state;
      if(((unsigned long)millis() - debounce) >= t_debounce){
        debounce = millis();
        if(pressed != flags.read(ROX_BTN_STATE)){

          holdDebounce = millis();
          flags.write(ROX_BTN_STATE, pressed);
          flags.write(ROX_BTN_STATE_CHANGED, true);
          flags.write(ROX_BTN_STATE_HELD, pressed);

          if(onUpdateCallback){
            // use updateWithCallback to also set a hold time, otherwise the
            // default hold time 500ms is used
            read(ROX_BTN_HOLD_THRESH);
          }
          return true;
        }
      }
      return false;
    }
    void updateWithCallback(bool state, uint16_t t_debounce=50, bool t_logic=LOW, uint16_t t_hold=ROX_BTN_HOLD_THRESH, bool ignoreAfterHold=false){
      if(update(state, t_debounce, t_logic) && onUpdateCallback){
        read(t_hold, ignoreAfterHold);
      }
    }
    uint8_t read(uint16_t t_hold=ROX_BTN_HOLD_THRESH, bool ignoreAfterHold=false){
      if(stateChanged()){
        if(held(t_hold)){
          if(onUpdateCallback){
            onUpdateCallback(ROX_HELD);
          }
          return ROX_HELD;
        } else {
          if(doublePressed()){
            if(onUpdateCallback){
              onUpdateCallback(ROX_DOUBLE);
            }
            return ROX_DOUBLE;
          } else if(isPressed()){
            if(onUpdateCallback){
              onUpdateCallback(ROX_PRESSED);
            }
            return ROX_PRESSED;
          } else if(released(ignoreAfterHold)){
            if(onUpdateCallback){
              onUpdateCallback(ROX_RELEASED);
            }
            return ROX_RELEASED;
          }
        }
      }
      return ROX_NONE;
    }
    void setDoublePressThreshold(uint16_t t_thres){
      doublePressTime = t_thres;
    }
    bool doublePressed(bool allowRelease=false){
      bool state = flags.toggleIfTrue(ROX_BTN_STATE_DBL_TRIGGERED);
      if(state && !allowRelease){
        flags.on(ROX_BTN_STATE_DBL_IGNORE_REL);
      }
      return state;
    }
    //ROX_BTN_STATE_DBL_PRESS_STRT
    bool pressed(){
      bool state = stateChanged() && isPressed();
      if(state){
        if(setDblPress()){
          return false;
        }
      }
      return state;
    }
    // you can pass a boolean to released() this will determine weather the
    // release will be triggered after a hold has been triggered
    bool released(bool ignoreAfterHold=false){
      bool state = stateChanged() && !isPressed();
      bool holdTriggered = flags.read(ROX_BTN_STATE_HOLD_TRIGGERED);
      bool dblTriggered = flags.read(ROX_BTN_STATE_DBL_IGNORE_REL);
      if(state){
        flags.off(ROX_BTN_STATE_HOLD_TRIGGERED);
        flags.off(ROX_BTN_STATE_DBL_IGNORE_REL);
      }
      if(state && ignoreAfterHold && holdTriggered){
        return false;
      }
      if(state){
        setDblRelease();
        if(dblTriggered){
          return false;
        }
      }
      return state;
    }
    bool held(uint16_t holdTime=ROX_BTN_HOLD_THRESH){
      bool state = isPressed() && btnHeld() && ((unsigned long)(millis()-holdDebounce)>=holdTime);
      if(state && !flags.read(ROX_BTN_STATE_HOLD_TRIGGERED)){
        flags.on(ROX_BTN_STATE_HOLD_TRIGGERED);
        flags.off(ROX_BTN_STATE_DBL_ACTIVE);
        return true;
      }
      return false;
    }
    bool latched(){
      return stateChanged() && isPressed();
    }
    bool unlatched(){
      return stateChanged() && !isPressed();
    }
    bool getCurrentState(){
      return isPressed();
    }
};

#endif
