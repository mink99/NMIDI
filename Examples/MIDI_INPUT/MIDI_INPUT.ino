




//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.


#include <NMIDI.h>
using namespace nmidi;

//Create new instance of ArduMIDI library:
//ArduMIDI(SerialObject (Serial, Serial1...), ListenChannel (CH1-CH16 or CH_ALL))
MidiPort midi = MidiPort(Serial1);

void setup() {
  //Turn of MEGA 2560's anoying always-on status LED:
  pinMode(13, OUTPUT); digitalWrite(13, LOW);

  //Begin MIDI library:
  Serial1.begin(31250);


  //Begin regular Serial:
  Serial.begin(9600);
  Serial.print("MIDI TEST ");
  midi.begin();

  //Control h: FORWARD_OFF, FORWARD_ALL, FORWARD_OTHER (default), FORWARD_SELF
  // midi.setThrow data is forwarded:
  //ThruModesuMode(FORWARD_ALL);


  midi.handleMidiEvent(processData);
  midi.handleNoteOn(_handleNoteON);
  midi.handleNoteOff(_handleNoteOFF);
  midi.handleProgramChange(_handleProgramChange);
}

void loop() {
  digitalWrite(13, midi.keysPressed());   // turn the LED on (HIGH is the voltage level)
 
}
void serialEvent1()
{
 midi.scanForData();
}

boolean processData(uint8_t port, CommandType cmd, Channel ch, byte data[], uint8_t msgLen) {
  //Print MIDI Event Data:
  Serial.print("***** ");
  Serial.print(midi.commandTypeToString(cmd));
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
void _handleNoteON(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note ON");
  Serial.println(decodeNote2(note));

}
void _handleNoteOFF(uint8_t port, Channel ch, byte note, byte velocity)
{
  Serial.print("***** Note OFF");
  Serial.println(decodeNote2(note));
}
void _handleProgramChange(uint8_t port, Channel ch, byte pgm)
{
  Serial.print("***** Program Change ");
  Serial.println(pgm);
}
