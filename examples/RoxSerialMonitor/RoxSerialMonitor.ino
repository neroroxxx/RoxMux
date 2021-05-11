/*
  This utility is used to read input from the serial Monitor

  You must declare a char array that is used as a buffer for incoming text
  When the "Send" button on the arduino serial monitor is clicked the entire
  string read is passed to the buffer.

  Use at your own risk.
*/
#include <RoxMux.h>

// buffer char array size, 32 character long string can be read from serial monitor
const uint16_t size = 33;
// buffer
char message[size];

// the buffer will consist of unsigned bytes, 40 bytes total.
RoxSerialMonitor serMon;


void setup(){
  delay(100);
  // this class uses the Serial object, you must call the begin() method
  // of the RoxSerialMonitor which will then call Serial.begin(),
  // however, if you want to call the Serial.begin somewhere else then jsut call
  // the begin() method of RoxSerialMonitor without any parameter, if you
  // want this function to also cal the Serial.begin() then pass a baud rate.

  // same as Serial.begin(115200); you can pass any acceptable baud rate
  serMon.begin(115200);

  // if the Serial.begin method is being called elsewhere in your code
  //serMon.begin();

  Serial.println("RoxSerialMonitor Example");
}

// GO AHEAD AND TYPE ANYTHING ON YOUR SERIAL MONITOR AND IT WILL BE ECHOED.
void loop(){
  // call this method in your loop, it will return true when the input has been
  // fully added to the buffer, parameters include the buffer string and it's max size
  if(serMon.read(message, size)){
    // If your type abc in the serial monitor an additional message will be sent
    // you can use ROX_STR_MATCH to compare 2 strings.
    // this is useful if you want to send commands to your sketch
    if(ROX_STR_MATCH(message, "abc")){
      Serial.println("You tested the abc command!");
    } else {
      Serial.print("Received: ");
      Serial.println(message);
    }
  }
}
