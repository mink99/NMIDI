




//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.


#define PORTX Serial1
#define PORTY Serial2



#include <NMIDI.h>

using namespace nmidi;

//Create new instance of nMidi library:

MidiPort portA(PORTX);
MidiPort portB(PORTY);

void setup() {
  //Turn of MEGA 2560's anoying always-on status LED:

  //Begin MIDI library:
  PORTX.begin(31250);
  PORTY.begin(31250);

  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
  //portB.begin(2);
  //portB.enableRunningStatus(false);
  Serial.begin(9600);
  Serial.println("MIDI TEST Input");
  Serial.print("Serial port:");
  Serial.print(PORTX);
  Serial.print(" with id:");
  Serial.println(portA.getPortID());
//  Serial.print("Serial Port:");
//  Serial.print(PORTY);
//  Serial.print(" with id:");
//  Serial.println(portB.getPortID()); 
  Serial.println("---------------------------------------");
  

  portA.handleMidiEvent(processData);
  //portB.handleMidiEvent(processData);
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
//
  portA.handleProgramChange(_handleProgramChange);

}

void loop() {
  portA.scanForData();
  //portB.scanForData();
}

boolean processData(uint8_t port, CommandType cmd, Channel ch, byte data[], uint8_t msgLen) {
  //Print MIDI Event Data:
  Serial.print(port);
  Serial.print(":");
  Serial.print(portA.commandTypeToString(cmd));
  Serial.print(" (");
  Serial.print(msgLen);
  Serial.print(")CH:  ");
  if (ch == CH_ALL) Serial.print("[N/A]");
  else Serial.print(ch + 1);
  Serial.print("[");
  for (int i = 0; i < msgLen; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("]");
  return true;
}
void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note ON (");
  Serial.print(port);
  Serial.print(",");
  Serial.print(ch + 1);
  Serial.print("):");
  Serial.println(decodeNote2(note));
  portA.sendNoteOn(ch, note, velocity);
}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note OFF(");
  Serial.print(ch + 1);
  Serial.print("):");
  Serial.println(decodeNote2(note));
  portA.sendNoteOff(ch, note, velocity);
}
void _handleProgramChange(uint8_t port, Channel ch, byte pgm)
{
  Serial.print("***** Program Change ");
  Serial.println(pgm);
}
