//NMIDI "Test" Example, by Pecacheu.

//Simple test to make sure your MIDI OUT port is wired correctly.

#include <HardwareSerial.h> //<-- Not Necessary, But Recomended for Stability Across Arduino IDE Versions.


#include <NMIDI.h>

using namespace nmidi;
boolean onOff = false;


//Create new instance of NMIDI library:
MidiPort port = MidiPort(Serial1);

void setup() {
  Serial.begin(9600);
  //Initialize MIDI Libary:
  Serial1.begin(31250);
  port.begin();
  port.enableRunningStatus();
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), pinISR, RISING  ); //(Empfohlen)
}

void loop() {


}
void pinISR()
{
  onOff = !onOff;
  Serial.println("*");
  if (onOff)    port.sendNoteOn(CH1, 72, 90);
  else port.sendNoteOff(CH1, 72, 90);
}
