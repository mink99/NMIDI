#include <ntools.h>

#include <NMIDI.h>
using namespace nmidi;

#include <stream.h>
#include "DebugStream.h"

DebugStream debugStream;
//Create new instance of nMidi library:

MidiPort portA(debugStream);

/* EXAMPLE
byte d[] = 
{
  NOTE_ON + CH1,   // Message is NOTE_ON  Channel is CH1
  44,              // note is #44
  100,             // velocity is 100
  
};
*/
byte d[] = 
{
  NOTE_ON + CH4,   
  44,
  127,
  NOTE_OFF + CH4,   
  44,
  127,
  STOP,
  PROG_CHANGE + CH7,
  62
  
};
#include "handlers.h"
void setup()
{
  Serial.begin(9600);



//-----------------------------------
//provide the message to the stream
  int sz = sizeof d;  
  debugStream.addBytes(sz,d);
  
//-----------------------------------
   Serial.println("\n*************************************************");
  //portA.handleMidiEvent(_processData);
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
  portA.handleProgramChange(_handleProgramChange);

  portA.begin(1);
//-----------------------------------
 
  portA.scanForDataA();
}

void loop()
{

}
