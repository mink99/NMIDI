
//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.



#define PORTX Serial1



#include <NMIDI.h>

using namespace nmidi;

//Create new instance of nMidi library:

MidiPort portA(PORTX);

int keysDown = 0;

void setup() {
  
  
  Serial.begin(9600);
  while(!Serial) {};
  //Begin MIDI library:
  PORTX.begin(31250);
  cout << s;

  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
    
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
//  
  portA.handleTimingClock(_handleClock);

}

void loop() {
  portA.scanForData();
}


void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  keysDown++;
  portA.sendNoteOn(ch, note+12, velocity);
}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  
  portA.sendNoteOff(ch, note+12, velocity);
  keysDown--;
}
void _handleClock(uint8_t port)
{
  portA.timingClock();
}
