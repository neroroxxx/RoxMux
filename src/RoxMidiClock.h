/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  This is a simple MIDI Clock that doesn't use interrupts, I recommend using it
  on faster boards like Teensy 3+.
  The clock can be set to Master or Slave, it uses callbacks to

  Use at your own risk.
*/
#ifndef RoxMidiClock_h
#define RoxMidiClock_h

#define MICROS_PER_SECOND 0x3938700

class RoxMidiClock {
private:
  void (*masterCallback)();
  void (*tempoCallback)(uint16_t t_newTempo);
  void (*beatCallback)();
  unsigned long masterTimer = 0;
  unsigned long masterIntervals = 0;
  unsigned long slaveTimeout = 0;
  unsigned long slaveLastTick = 0;
  unsigned long notifyTimer = 0;
  unsigned long tapTimer = 0;
  uint16_t tmpBpm = 0;
  uint16_t bpm = 0;
  uint8_t ticks = 0;
  bool master = false;
  void triggerBeatCallback(){
    if(beatCallback){
      beatCallback();
    }
  }
  void triggerMasterTickCallback(){
    if(masterCallback){
      masterCallback();
    }
  }
  void triggerTempoChangeCallback(){
    if(tempoCallback){
      tempoCallback(bpm);
    }
  }
public:
  RoxMidiClock(){
    masterCallback = 0;
    tempoCallback = 0;
    beatCallback = 0;
  }
  void begin(bool t_master=false){
    setMode(t_master);
    ticks = 0;
    masterTimer = micros();
    setMasterBpm(120);
  }
  void onMasterTickCallback(void (*fptr)()){
    masterCallback = fptr;
  }
  void onTempoChangeCallback(void (*fptr)(uint16_t t_newTempo)){
    tempoCallback = fptr;
  }
  void onBeatCallback(void (*fptr)()){
    beatCallback = fptr;
  }
  void setMasterBpm(uint16_t t){
    if(t>=30 && t<=300){
      masterIntervals = (MICROS_PER_SECOND/t)/24;
      // start a timer to notify that the tempo has changed
      if(bpm != t){
        bpm = t;
        notifyTimer = millis();
      }
    }
  }
  void tap(){
    if(!isMaster()){
      return;
    }
    if(tapTimer==0){
      // initial tap
      tapTimer = millis();
      return;
    }
    unsigned long t = (unsigned long)millis()-tapTimer;
    // t is the time between taps, RoxMidiClock supports a range from 30 to 300bpm
    // that means we can have a time between taps of 200ms to 2000ms
    // anything out of that range is ignored and resets the tap.
    // This is so that when you stop doing a tap, the next time you start another tap
    // the tempo will be calculated starting from that tap.
    if(t<100 || t > 2100){
      // if it's more than 2500ms between taps
      tapTimer = 0;
      return;
    } else if(t>2000){
      // however if we are between 2000 and 2100 we'll go to the slowest tap value
      // which is 30bpm which is milliseconds it's 2000ms
      t = 2000;
    } else if(t<200){
      // if we are between 100 and 200ms we'll go to the fastest time which is 200ms
      t = 200;
    }
    uint16_t _bpm = 60000/t;
    if(_bpm>=30 && bpm<=300){
      setMasterBpm(_bpm);
    }
    tapTimer = 0;
  }
  bool isMaster(){
    return master;
  }
  bool isSlave(){
    return !isMaster();
  }
  uint16_t getBpm(){
    return bpm;
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
    ticks = 0;
    slaveTimeout = millis();
  }
  void update(){
    if(isMaster()){
      if(micros()-masterTimer >= masterIntervals){
        masterTimer = micros();
        triggerMasterTickCallback();
        ticks++;
        if(ticks>=24){
          triggerBeatCallback();
          ticks = 0;
        }
      }
    }
    if(notifyTimer>0 && millis()-notifyTimer>=1000){
      triggerTempoChangeCallback();
      notifyTimer = 0;
    }
    if(isSlave() && slaveTimeout>0 && (unsigned long)millis()-slaveTimeout>=1000){
      bpm = 0;
      triggerTempoChangeCallback();
      slaveTimeout = 0;
    }
  }
  // call this method whenever a midi clock message (0xF8) is received
  void tick(){
    if(isMaster()){
      return;
    }
    ticks++;
    if(ticks>=24){
      ticks = 0;
      tmpBpm = (uint16_t)ceil(60000.0/(millis()-(slaveLastTick+0.0)));
      slaveLastTick = millis();
      if(tmpBpm!=bpm){
        bpm = tmpBpm;
        // start a timer to notify that the tempo has changed
        if(notifyTimer==0){
          notifyTimer = millis();
        }
      }
      triggerBeatCallback();
    }
    slaveTimeout = millis();
  }

};
#endif
