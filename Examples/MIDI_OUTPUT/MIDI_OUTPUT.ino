//NMIDI "Test" Example, by Pecacheu.

//Simple test to make sure your MIDI OUT port is wired correctly.

#include <HardwareSerial.h> //<-- Not Necessary, But Recomended for Stability Across Arduino IDE Versions.


#include <NMIDI.h>

using namespace nmidi;

//Create new instance of NMIDI library:
MidiPort portA = MidiPort(Serial1);
MidiPort portB = MidiPort(Serial2);

void setup() {

  //Initialize MIDI Libary:
  Serial1.begin(31250);
  Serial2.begin(31250);
  Serial.begin(9600);
  portA.begin();
  portA.enableRunningStatus();
  portB.begin();
  portB.enableRunningStatus();
}

void loop() {

  //  //Generate random note:
  byte rand = random(42, 80);
  //Turn note on, then off again:


  for (int i = 1; i < 12; i++)
  {
    portA.sendNoteOn(CH1, rand + i, 90);
    portB.sendNoteOn(CH2, rand + i + 12, 90);
    Serial.println(rand + i);

    delay(120);
    portA.sendNoteOff(CH1, rand + i, 90);
    portB.sendNoteOff(CH2, rand + i + 12, 90);
  }
  delay(120);
  
  //port.sendBankChange(CH1,1, 32);

  //port.sendAllSoundsOff(CH1);

  //port.sendResetAll(CH1);

  //  port.sendLocalControl(CH1,true);
  //  port.sendLocalControl(CH1,false);

  //port.sendAllNotesOff(CH1);

  //port.sendOmniOn(CH1);

  //port.sendOmniOff(CH1);

  // port.sendMonoOn(CH1,4);

  //port.sendPolyOn(CH1);

  //port.sendSongSelect(7);

  //port.sendTuneRequest();

  // for (int i = 0; i < 32; i++)
  // {
  //  port.sendControlChange(CH1,i,62);
  //  delay(300);
  //
  // }

  // for (int i = 0; i < 32; i++)
  // {
  //  port.sendControlChangeHi(CH2,i,12344);
  //  delay(300);
  //
  // }

  //  for (int i = 0; i < 32; i++)
  //  {
  //    port.sendProgramChange(CH1, i);
  //    delay(300);
  //
  //  }
}
