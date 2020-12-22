*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxMidiClock

RoxMidiClock is a simple MIDI Clock Generator and Slave, it can be set to either Master or Slave.

It works with callbacks, when set to master a callback will tell your sketch when to send the MIDI Clock Message out, these are sent out 24 times per quarter note.

There is also a callback, for Beats, it its triggered with every beat, with it you can pulse an LED to see the speed of the clock.

It also has a Tempo Changed callback which will notify your sketch whever the tempo has been recalculated (mainly for slave mode tho it can be used when switching between modes) which can be used to print the current Beats Per Minute to a display.

When in slave mode, RoxMidiClock will read the ticks and determined the current BPM, it will also trigger a callback with each beat.

In slave mode you call the .tick() method which will whenever teh 0xF8 MIDI status  is received.

See the Example for more info.

```c++
#include <RoxMux.h>
#include <MIDI.h>

#define BUTTON_PIN 2
#define LED_PIN 13

MIDI_CREATE_DEFAULT_INSTANCE();

// for this example we'll use the built-in led on pin 13
RoxMidiClock clock;
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
  MIDI.setHandleClock(tick);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.begin();

  led.begin(LED_PIN);
  led.setMode(ROX_BLINK);

  // .begin() takes an boolean argument, this will tell it to be master (true)
  // or slave (false)
  clock.begin(true);
  // set the default bpm for the master clock to 120BPM
  clock.setMasterBpm(120);


  // you can also change it from master to slave on the fly with .setMode()
  //clock.setMode(false);// set it to slave, set it to true to be master

  // set your callbacks, there are 3 available
  // .onMasterTickCallback() this callback doesnt have arguments, this callback
  // should send out the MIDI 0xF8 message
  clock.onMasterTickCallback(sendMidiClock);
  // .onTempoChangeCallback() this callback has a uint16_t argument
  // which has the newest tempo detected when on slave, or the newly set tempo
  // when on master, this is also the tempo calculated when using .tap()
  clock.onTempoChangeCallback(onTempoChanged);
  // .onBeatCallback() this callback has no argument, it's triggered every
  // after 24 0xF8 are received and then reset again, this will give you the
  // quarter note, use it to blink an LED
  clock.onBeatCallback(onBeat);
}
void tick(){
  clock.tick();
}
void sendMidiClock(){
  MIDI.sendRealTime(midi::Clock);
}
void onTempoChanged(uint16_t t_newTempo){
  // uncomment the line below if you are using Teensy to see the new
  // tempo detected on the serial monitor
  // If you're using an arduino UNO then leave them commented.
  //Serial.print("New Tempo: "); Serial.println(t_newTempo);
}
void onBeat(){
  led.pulse();
}
void loop(){
  MIDI.read();
  clock.update();
  led.update(75);
  button.update(digitalRead(BUTTON_PIN), 50, LOW);
  if(button.pressed()){
    clock.tap();
  }
}
```
