




//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.


#define PORTY Serial
#define PORTX Serial1



#include <NMIDI.h>

using namespace nmidi;

//Create new instance of nMidi library:

MidiPort portA(PORTX);


void setup() {
  //Turn of MEGA 2560's anoying always-on status LED:

  //Begin MIDI library:
  PORTX.begin(31250);
  

  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
  
  PORTY.begin(9600);
  PORTY.println("MIDI TEST Input");
  PORTY.print("Serial port:");
  PORTY.print(PORTX);
  PORTY.print(" with id:");
  PORTY.println(portA.getPortID());

  PORTY.println("---------------------------------------");
  

  portA.handleMidiEvent(processData);
  //portB.handleMidiEvent(processData);
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
//
  portA.handleProgramChange(_handleProgramChange);
  portA.handleTimingClock(_handleClock);

}

void loop() {
  portA.scanForData();
  //portB.scanForData();
}

boolean processData(uint8_t port, CommandType cmd, Channel ch, byte data[], uint8_t msgLen) {
  //Print MIDI Event Data:
  PORTY.print(port);
  PORTY.print(":");
  PORTY.print(portA.commandTypeToString(cmd));
  PORTY.print(" (");
  PORTY.print(msgLen);
  PORTY.print(")CH:  ");
  if (ch == CH_ALL) PORTY.print("[N/A]");
  else PORTY.print(ch + 1);
  PORTY.print("[");
  for (int i = 0; i < msgLen; i++)
  {
    PORTY.print(data[i], HEX);
    PORTY.print(" ");
  }
  PORTY.println("]");
  return true;
}
void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  PORTY.print("***** Note ON (");
  PORTY.print(port);
  PORTY.print(",");
  PORTY.print(ch + 1);
  PORTY.print("):");
  PORTY.println(decodeNote2(note));
  portA.sendNoteOn(ch, note, velocity);
}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  PORTY.print("***** Note OFF(");
  PORTY.print(ch + 1);
  PORTY.print("):");
  PORTY.println(decodeNote2(note));
  
}
void _handleProgramChange(uint8_t port, Channel ch, byte pgm)
{
  PORTY.print("***** Program Change ");
  PORTY.println(pgm);
}
void _handleClock(uint8_t port)
{
  portA.timingClock();
}
