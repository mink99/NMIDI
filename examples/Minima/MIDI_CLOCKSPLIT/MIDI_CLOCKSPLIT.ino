
//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.
#define PORTX Serial1



#include <NMIDI.h>
#include <nmidi_clock.h>
#include <nmidi_clksplit.h>

using namespace nmidi;
//--------------------------------------------
class BarCounter : public ClockFilter
{
  public:
  void on1() override
  {
    Serial.print(" bar ");
    Serial.print(bars());
    Serial.print(":");
    Serial.println(ticks());
  }
};
//--------------------------------------------

//Create new instance of nMidi library:

MidiPort portA(PORTX);
BarCounter barCounter;

void setup() {

  //Begin MIDI library:
  PORTX.begin(31250);
  

  //Begin regular Serial:
  portA.begin(1);
  portA.enableRunningStatus(false);
  
  Serial.begin(9600);
  while (!Serial) {};
  
  Serial.println("MIDI TEST ClockSplitter");
  Serial.print("Serial port:");
  Serial.print(PORTX);
  Serial.print(" with id:");
  Serial.println(portA.getPortID());

  Serial.println("---------------------------------------");
  
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
  portA.handleProgramChange(_handleProgramChange);
  portA.handleTimingClock(_handleClock);
  getHardwareClock(&portA);
  barCounter.reset();

  CLK.init();  
  CLK.setBPM(120);  
  CLK.start();

}

void loop() {
  portA.scanForData();
 
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
void _handleClock(uint8_t port)
{
  barCounter.clockEvent();
}
