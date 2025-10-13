//ArduMIDI "Print" Example, by Pecacheu.

//Print MIDI Events to the USB serial.

//Note: This sketch requires an Arduino with multiple serial ports, such as an Arduino MEGA.

#include <NMIDI.h>
using namespace nmidi;


//Create new instance of ArduMIDI library:
//ArduMIDI(SerialObject (Serial, Serial1...), ListenChannel (CH1-CH16 or CH_ALL))
MidiPort port = MidiPort(Serial1);

void setup() {


  pinMode(13, OUTPUT); digitalWrite(13, LOW);
  Serial.begin(9600);
  //Initialize MIDI Libary:
  Serial1.begin(31250);
  port.begin();
  port.enableRunningStatus();
  port.handleMidiEvent(processData);
  port.setThruMode(FORWARD_ALL);
}

void loop() {
  //Call this function as frequently as possible for MIDI reading to work:

}

boolean processData(uint8_t _port, CommandType cmd, Channel ch, byte data[], uint8_t datalen) {
  //Print MIDI Event Data:
  Serial.print(port.commandTypeToString(cmd));
  Serial.print(", ");
  if (ch == CH_ALL) Serial.print("[N/A]");
  else Serial.print(ch + 1);
  Serial.print(", {");
  for (int i = 0; i < datalen; i++) {
    Serial.print(data[i]);
    if (i + 1 < datalen) Serial.print(",");
  }
  Serial.println("}");
}
void serialEvent1()
{
  port.scanForData();
}
