#include <RotaryEncoder.h>

#define PIN_IN1 2
#define PIN_IN2 3

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

void checkPosition()
{
  encoder.tick(); // just call tick() to check the state.  
}

#include <NMIDI.h>
#include <nmidi_clock.h>


using namespace nmidi;



int l = 0;
int beatCounter = 0;

MidiPort portA = nmidi::MidiPort(Serial1);
getHardwareClock(&portA);

void pr(const String &s)
{
  Serial.println( s);
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {};
  Serial1.begin(31250);
   //while (!Serial1) {};
  pinMode(LED_BUILTIN, OUTPUT);
  pr("***************** MIDI internal Clock Test ******************");


  portA.begin();


  portA.enableRunningStatus(false);
  
  portA.handleTimingClock(_handleClock);
  portA.handleStart(_handleStart);
  portA.handleStop(_handleStop);

  CLK.init();
  CLK.setBPM(120);
  CLK.start();
  pr("***************** Test enter Loop  ******************");
  //encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);
  encoder.setMinMaxPosition(40,220);
  encoder.setPosition(120);
}

void loop() {
  static int pos = 0;

  portA.scanForData();

  int newPos = encoder.getPosition();
  if (pos != newPos)
  {
    Serial.print("---------");
    Serial.println(pos);
    CLK.setBPM(newPos);
    pos = newPos;
  }


}
void _handleClock(uint8_t port)
{
  portA.timingClock();
  beatCounter++;
  //pr("---- _handleClock CLK ");

  if (beatCounter % 6 == 0)
  {
    //pr("---- _handleClock CLK 1/16");
  }
  if (beatCounter % 12 == 0)
  {
    //pr("-- _handleClock CLK 1/8");
  }
  if (beatCounter % 24 == 0)
  {
    //pr("- _handleClock CLK 1/4");
  }
  if (beatCounter % 48 == 0)
  {
    //pr("- _handleClock CLK 1/2");
  }
  if (beatCounter % 96 == 0)
  {
    //pr("- _handleClock CLK 1/1");
    //pr(String(CLK.getBPM()));
    beatCounter = 0;
  }
  
}
void _handleStart(uint8_t port)
{
  pr("_handleStart ---- start");
}
void _handleStop(uint8_t port)
{
  pr("_handleStop ---- stop");
}
