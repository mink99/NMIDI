#include <ntools.h>
#include <ndebug.h>







//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.


#define PORTX Serial1
#define PORTY Serial2



#include <NMIDI.h>
#include <nmidi-if.h>
using namespace nmidi;


//Create new instance of nMidi library:

MidiPort portA = MidiPort(PORTX);
MidiPort portB = MidiPort(PORTY);

//class Hdld : INoteHandler , IMidiEventHandler, IProgramChangeHandler
class Hdld : public INoteOnHandler,  public INoteOffHandler , public IMidiEventHandler, public IProgramChangeHandler
{
    boolean _handleMidiEvent(uint8_t port, CommandType cmd, Channel ch, byte data[], uint8_t msgLen) {
      //Print MIDI Event Data:

      dbg_print(port);
      dbg_print(":");
      dbg_print(portA.commandTypeToString(cmd));
      dbg_print(" (");
      dbg_print(msgLen);
      dbg_print(")CH:  ");
      if (ch == CH_ALL) dbg_print("[N/A]");
      else dbg_print(ch + 1);
      dbg_print("[");
      for (int i = 0; i < msgLen; i++)
      {
        dbg_print(data[i], HEX);
        dbg_print(" ");
      }
      dbg_println("]");
      return true;
    }
    void _handleNoteOn(uint8_t port, Channel ch, byte note, byte velocity)
    {
      dbg_print("***** Note ON (");
      dbg_print(port);
      dbg_print(",");
      dbg_print(ch + 1);
      dbg_print("):");
      dbg_println(decodeNote2(note));
      portA.sendNoteOn(ch, note, velocity);
    }
    void _handleNoteOff(uint8_t port, Channel ch, byte note, byte velocity)
    {
      dbg_print("***** Note OFF(");
      dbg_print(ch + 1);
      dbg_print("):");
      dbg_println(decodeNote2(note));
      portA.sendNoteOff(ch, note, velocity);
    }
    void _handleProgramChange(uint8_t port, Channel ch, byte pgm)
    {
      dbg_print("***** Program Change ");
      dbg_println(pgm);
    }
};


Hdld Handler;


void setup() {
  //Turn of MEGA 2560's anoying always-on status LED:

  //Begin MIDI library:
  PORTX.begin(31250);
  PORTY.begin(31250);


  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
  portA.setThruMode(FORWARD_SELF);
  ;
  dbg_begin(9600);
  dbg_println("MIDI Forward TEST ");
  dbg_print("Serial port:");
  dbg_print(PORTX);
  dbg_print(" with id:");
  dbg_println(portA.getPortID());
  dbg_println("---------------------------------------");


    portA.handleMidiEvent(&Handler);
    portA.handleNoteOn(&Handler);
    portA.handleNoteOff(&Handler);
  //
    portA.handleProgramChange(&Handler );

}

void loop() {
  portA.scanForData();



}
