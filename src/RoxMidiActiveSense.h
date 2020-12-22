/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a simple MIDI Clock that doesn't use interrupts, I recommend using it
  on faster boards like Teensy 3+.
  The clock can be set to Master or Slave, it uses callbacks to

  Use at your own risk.
*/
#ifndef RoxMidiActiveSense_h
#define RoxMidiActiveSense_h

#include <RoxFlags.h>

class RoxMidiActiveSense {
private:
  void (*masterCallback)();
  void (*stateChangeCallback)(bool t_status);
  unsigned long masterTimer = 0;
  unsigned long slaveTimeout = 0;
  bool master = false;
  bool status = false;
  // trigger when the Active sense 0xFE should be sent
  void triggerMasterTickCallback(){
    if(masterCallback){
      masterCallback();
    }
  }
  void triggerStateChangeCallback(){
    if(stateChangeCallback){
      stateChangeCallback(status);
    }
  }
  void setStatus(bool t_status){
    status = t_status;
  }
  void updateStatus(bool t_status){
    setStatus(t_status);
    triggerStateChangeCallback();
  }
public:
  RoxMidiActiveSense(){
    masterCallback = 0;
    stateChangeCallback = 0;
  }
  void begin(bool t_master=false){
    setMode(t_master);
    masterTimer = millis();
  }
  void update(){
    if(isMaster()){
      if(millis()-masterTimer >= 295){
        masterTimer = millis();
        triggerMasterTickCallback();
        if(!status){
          updateStatus(true);
        }
      }
    } else if(slaveTimeout>0 && (unsigned long)millis()-slaveTimeout>=600){
      updateStatus(false);
      slaveTimeout = 0;
    }
  }
  // call this method whenever a midi clock message (0xF8) is received
  void tick(){
    if(!isMaster()){
      if(!status){
        updateStatus(true);
      }
      slaveTimeout = millis();
    }
  }
  void onMasterTickCallback(void (*fptr)()){
    masterCallback = fptr;
  }
  void onStateChangeCallback(void (*fptr)(bool t_status)){
    stateChangeCallback = fptr;
  }
  bool isMaster(){
    return master;
  }
  bool isSlave(){
    return !master;
  }
  void setAsMaster(){
    setMode(true);
  }
  void setAsSlave(){
    setMode(false);
  }
  void toggleMode(){
    setMode(!master);
  }
  void setMode(bool t_master=false){
    master = t_master;
    slaveTimeout = millis();
  }

};
#endif
