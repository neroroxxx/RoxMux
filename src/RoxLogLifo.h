/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

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

  Use at your own risk.
*/
#ifndef RoxLogLifo_H
#define RoxLogLifo_H

#include "RoxFlags.h"

#define ROX_LOG_LIFO_FLAG_UPDATE 0

// this class is a the template, the first argument is the type of object for the array
// the _maxLength is the maximum number of elements the buffer will ever have.

template <typename T, uint16_t _maxLength>
class RoxLogLifo {
private:
  T buffer[_maxLength];
  uint16_t bufferLen = 0;
  uint16_t head = 0;
  RoxFlags <uint8_t> flags;
  void (*callback)(T t_data, uint16_t t_count);
  void triggerCallback(T item, uint16_t t_count){
    if(callback){
      callback(item, t_count);
    }
  }
public:
  RoxLogLifo(){
    flags.reset();
  }
  void begin(void (*fptr)(T t_data, uint16_t t_count)){
    callback = fptr;
  }
  void append(T data){
    buffer[head] = data;
    head++;
    if(head>=_maxLength){
      head = 0;
    }
    if(bufferLen<_maxLength){
      bufferLen++;
    }
    // update flag to trigger an update
    flags.on(ROX_LOG_LIFO_FLAG_UPDATE);
  }
  // get the current size of the buffer, once it reaches the max it will
  // always return the same value
  uint16_t getLength(){
    return bufferLen;
  }
  // reset the buffer, effectively clearing the log
  void clear(){
    bufferLen = 0;
    head = 0;
  }
  void triggerUpdate(){
    // update flag to trigger an update
    flags.on(ROX_LOG_LIFO_FLAG_UPDATE);
  }
  unsigned long getBufferSize(){
    return sizeof(buffer)/sizeof(T);
  }
  // update should be added to your sketch's loop function
  // @t_startingAt: where should the list start at.
  //                this is useful if you want to look at previous messages.
  //                using something like a rotary encoder.
  // @t_listLength: how many items in the array should be display.
  //                this determines how many times the array will iterate
  //                this value is optional and it's default at 10 items.
  void update(uint16_t t_startingAt, uint16_t t_listLength=10){
    if(flags.toggleIfTrue(ROX_LOG_LIFO_FLAG_UPDATE)){
      if(t_startingAt >= bufferLen){
        t_startingAt = 0;
      }
      uint16_t printCount = 0;
      if(bufferLen>=_maxLength){
        for(uint16_t i = (head), index=0  ; i-- > 0 ;  index++){
          if(index<t_startingAt && bufferLen>t_listLength){ continue; }
          triggerCallback(buffer[i], printCount);
          printCount++;
          if(printCount>=t_listLength){
            break;
          }
        }
        if(printCount < t_listLength){
          for(uint16_t i = (bufferLen), index=0 ; i-- > 0 ;  index++){
            if(index<t_startingAt && bufferLen>t_listLength){ continue; }
            triggerCallback(buffer[i], printCount);
            printCount++;
            if(i==head || printCount>=t_listLength){
              break;
            }
          }
        }

      } else {
        for(uint16_t i = (bufferLen), index=0 ; i-- > 0 ; index++){
          if(index<t_startingAt && bufferLen>t_listLength){ continue; }
          triggerCallback(buffer[i], printCount);
          printCount++;
          if(printCount>=t_listLength){
            break;
          }
        }
      }
    }
  }
};
#endif
