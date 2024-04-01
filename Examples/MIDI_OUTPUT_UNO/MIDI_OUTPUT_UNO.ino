//NMIDI "Test" Example, by Mink.

#define POLY  


//Simple test to make sure your MIDI OUT port is wired correctly.

#include <HardwareSerial.h> //<-- Not Necessary, But Recomended for Stability Across Arduino IDE Versions.


#include <NMIDI.h>

using namespace nmidi;

//Create new instance of NMIDI library:
MidiPort port(Serial);

void setup() {

  //Initialize MIDI Libary:
  Serial.begin(31250);
  port.begin();
  port.enableRunningStatus(false);
  port.sendProgramChange(CH1, 0);

}

void loop() {

  //  //Generate random note:
  byte rand = random(42, 110);
  int note;
  //Turn note on, then off again:
#ifdef MONO  
  for (int i = 1; i < 12; i++)
  { 
    note = rand + i;
    port.sendNoteOn(CH1, note, 90);
    delay(120);
    port.sendNoteOff(CH1, note, 90);
  }
  delay(1200);
#endif
#ifdef POLY  
  for (int i = 1; i < 12; i++)
  { 
    note = rand + i;
    port.sendNoteOn(CH1, note, 90);
    delay(12);
    port.sendNoteOn(CH1, note+5, 90);
    delay(48);
    port.sendNoteOn(CH1, note+9, 90);
    delay(1200);
    port.sendNoteOff(CH1, note+9, 90);
    delay(120);
    port.sendNoteOff(CH1, note+5, 90);
    delay(48);
    port.sendNoteOff(CH1, note, 90);
    delay(1200);
  }
  delay(1200);
#endif
}
