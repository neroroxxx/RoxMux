/*
  Same as RoxEncoder Example but using the callback
*/
#include <RoxMux.h>

RoxEncoder encoder;

void setup(){
  // for this example pin 0 and 1 on my teensy are connected to an encoder
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  // start the serial monitor and print a line
  Serial.begin(115200);
  Serial.println("RoxEncoder Example");
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  encoder.begin();
  // see RoxEncoder example for info
  encoder.setTickSensitivity(2);
  // set a callback and leave the .update() method in your loop
  encoder.onUpdateCallback(onUpdate);
}
void loop(){
  // see RoxEncoder for more info
  encoder.update(digitalRead(0), digitalRead(1), 2, LOW);
}
// when the encoder is rotated it will trigged this callback
// @clockwise: is true if it was turned clockwise, otherwise false
// @ticks: this variable will return a number between 1 and x
//         when you turn the encoder slowly it will return 1
//         the faster you rotate the encoder, the higher this value will be
//         use this value to as a way to know how fast the encoder was turned
//         and how much to increase/decrease the value the encoder is controlling
//         The number of ticks depends on the sensitivity, and can be as high
//         as 9
void onUpdate(uint8_t ticks, bool clockwise){
  // encoder has been rotated
  Serial.print("Encoder Rotated: ");
  Serial.print(clockwise ? "Clockwise" : "Counter-Clockwise");
  Serial.print(", Ticks: ");
  Serial.println(ticks);
}
