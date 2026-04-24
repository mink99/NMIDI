#define PORTX Serial

#include <NMIDI.h>

using namespace nmidi;

//Create new instance of nMidi library:

MidiPort portA(PORTX);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //Begin MIDI library:
  PORTX.begin(31250);
  

  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
  

  digitalWrite(LED_BUILTIN, LOW);
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);

}

void loop() {
  portA.scanForData();
 
}

void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  if (velocity)  digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  digitalWrite(LED_BUILTIN, LOW);
}
