*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

[<< Repository](../README.md)

# RoxLogLifo

This is a utility I wrote to use with some of my displays to log incoming and
outgoing MIDI Messages.

LIFO stands for "Last In First Out" it's a circular buffer, meaning, you can
add data to the array and once the array is full the data is added to the beginning
of the array, effectively removing the oldest array element to add a new one.

In return the utility will always list the last message that was received first.

this is great when you want to log strings or anything like that, the newest item
to the array will always be listed first.

This LIFO works with a callback and an update() method, the update() method
 must always be called in the loop() function of your sketch.

 when you call the .append() method of the LIFO the data you pass it will
 be added to the buffer and the callback will be then triggered,
 the callback will iterate thru the buffer everytime you append to it.

 This will let you printout each item of the buffer directly to the display.


```c++

#include <RoxMux.h>

// the buffer will consist of unsigned bytes, 40 bytes total.
RoxLogLifo <uint8_t, 40> lifo;

// used by sketch
uint8_t counter = 0;

void setup(){
  delay(100);
  Serial.begin(115200);
  Serial.println("RoxLogLifo Example");
  // add the callback for the lifo
  lifo.begin(trigger);
}

void loop(){
  // update the lifo to listen for updates, display the list form index 0
  // and display a maximum of 5 messages
  lifo.update(0, 5);
  // read the serial monitor, anytime you click on the Send button of the Serial
  // monitor we'll append a value and display the updated list.
  bool ready = false;
  while(Serial.available() > 0){
    Serial.read();
    ready = true;
  }
  if(ready){
    lifo.append(counter);
    counter++;
    if(counter > 255){
      counter = 0;
    }
  }
}
// callback to render the list of items as they are added to it.
// @t_data: is the element of the array that is currently being rendered
// @t_count: the number of elements that have been passed since the last updated.
void trigger(uint8_t t_data, uint16_t t_count){
  if(t_count==0){
    Serial.println("----------------------");
  }
  Serial.print("t_data = ");
  Serial.print(t_data);
  Serial.print(", t_count ");
  Serial.println(t_count);
}
```
