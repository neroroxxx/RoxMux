*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxMidiActiveSense

RoxMidiActiveSense lets you run a master/slave MIDI Active sense, this utility works with callbacks so you can use any MIDI port like Serial/USB/USB HOST.

See the Example for more info.


```c++
#include <RoxMux.h>
#include <MIDI.h>

#define BUTTON_PIN 2
#define LED_PIN 13

MIDI_CREATE_DEFAULT_INSTANCE();

// for this example we'll use the built-in led on pin 13
RoxMidiActiveSense activeSense;
// button to switch between master and slave
RoxButton button;
// use the RoxLed to pulse the built in ld
RoxLed led;

void setup(){
  // little delay before starting
  delay(100);

  // init MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  MIDI.setHandleActiveSensing(tick);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.begin();

  led.begin(LED_PIN);
  led.setMode(ROX_PULSE);

  // .begin() takes an boolean argument, this will tell it to be master (true)
  // or slave (false)
  activeSense.begin(true);
  // you can also change it from master to slave on the fly with .setMode()
  //activeSense.setMode(false);// set it to slave, set it to true to be master

  // set your callbacks, there are 3 available
  // .onMasterTickCallback() this callback doesnt have arguments, this callback
  // should send out the MIDI 0xF8 message
  activeSense.onMasterTickCallback(sendMidiActiveSensing);
  // .onTempoChangeCallback() this callback has a uint16_t argument
  // which has the newest tempo detected when on slave, or the newly set tempo
  // when on master, this is also the tempo calculated when using .tap()
  activeSense.onStateChangeCallback(onStateChanged);
}
void tick(){
  activeSense.tick();
}
void sendMidiActiveSensing(){
  MIDI.sendRealTime(midi::ActiveSensing);
}
void onStateChanged(bool state){
  // make the led blink while it's sending or receiving active sense
  if(state){
    led.on();
  } else {
    led.off();
  }
}
void loop(){
  MIDI.read();
  activeSense.update();
  led.update(300);
  button.update(digitalRead(BUTTON_PIN), 50, LOW);
  if(button.pressed()){
    // toggle the active sense mode
    activeSense.toggleMode();
  }
}
```
