/*
  Example of RoxMidiClock as a master

  REQUIRES THE MIDI.h LIBRARY
  https://github.com/FortySevenEffects/arduino_midi_library

  Uses built-in serial for MIDI

  Wire a momentary button to act as a Tap Tempo
*/
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
