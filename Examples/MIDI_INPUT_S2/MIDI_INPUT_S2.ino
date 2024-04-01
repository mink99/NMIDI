#define DEBUG
#include <ndebug.h>






#define PORT Serial1
//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.


#include <NMIDI.h>
using namespace nmidi;

//Create new instance of NMidi library:

MidiPort portA;


void setup() {
  //Turn of MEGA 2560's anoying always-on status LED:
  pinMode(13, OUTPUT); digitalWrite(13, LOW);

  //Begin MIDI library:
  PORT.begin(31250);


  //Begin regular Serial:
  Serial.begin(9600);

  portA.begin(PORT,1);


  portA.handleMidiEvent(processData);
  portA.handleNoteOn(_handleNoteON);
  portA.handleNoteOff(_handleNoteOFF);
  portA.setThruMode(FORWARD_ALL);


  Serial.println("START MIDI READ");
}

void loop() {
  portA.scanForData();
  digitalWrite(13, portA.keysPressed());   // turn the LED on (HIGH is the voltage level)
  
}


boolean processData(uint8_t _port, CommandType cmd, Channel ch, byte data[], uint8_t msgLen) {
  //Print MIDI Event Data:
  Serial.print("***** ");
  Serial.print(_port);
  Serial.print(" ** ");
  Serial.print(portA.commandTypeToString(cmd));
  Serial.print(", ");
  if (ch == CH_ALL) Serial.print("[N/A]");
  else Serial.print(ch + 1);
  Serial.print(", {");
  for (int i = 0; i < msgLen; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("}");
  return true;
}
void _handleNoteON(uint8_t _port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note ON");
  Serial.println(decodeNote2(note));

}
void _handleNoteOFF(uint8_t _port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note OFF");
  Serial.println(decodeNote2(note));
}
void _handleProgramChange(uint8_t _port, Channel ch, byte pgm)
{
  Serial.print("***** Program Change ");
  Serial.println(pgm);
}
