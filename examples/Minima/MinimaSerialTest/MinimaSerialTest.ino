// #include <RotaryEncoder.h>

#include <NMIDI.h>
#include <nmidi_clock.h>
using namespace nmidi;

#define PORTX Serial1
MidiPort portA = MidiPort(PORTX);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(31250);


  while (!Serial) {};

  Serial.println("******** Serial ready ************");
  portA.begin();
  portA.enableRunningStatus(false);
  Serial.println("******** MIDI ready ************");
  Serial.println("\n MIDI TEST Output");
  Serial.print("Serial port:");
  Serial.print(PORTX);
  Serial.print(" with id:");
  Serial.println(portA.getPortID());
  Serial.flush();

}

void loop() {
  //Generate random note:
  byte rand = random(42, 80);
  //Turn note on, then off again:


  for (int i = 1; i < 12; i++)
  {
    int8_t _note = rand + i;
    digitalWrite(LED_BUILTIN, HIGH);
    portA.sendNoteOn(CH1, _note, 90);

    delay(120);
    digitalWrite(LED_BUILTIN, LOW);
    portA.sendNoteOff(CH1, _note, 90);

    delay(120);
   


  }
}
