
/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"


using namespace nmidi;

//Send 24 times per quarter note when SYNC is enabled:
void MidiPort::timingClock()
{
    writeMsg(TIMING_CLOCK);
}

//Start, continue, and stop playing:
void MidiPort::sendStart()
{
    writeMsg(START);
}
void MidiPort::sendContinue()
{
    writeMsg(CONTINUE);
}
void MidiPort::sendStop()
{
    writeMsg(STOP);
}

/** SPP Song Position Pointer.

This message consists of 3 bytes;
a status byte (decimal 242, hex 0xF2),
followed by two 7-bit data bytes (least significant byte first) forming a 14-bit value which specifies the number of "MIDI beats"
(1 MIDI beat = a 16th note = 6 clock pulses) since the start of the song.
This message only needs to be sent once if a jump to a different position in the song is needed.
Thereafter only realtime clock messages need to be sent to advance the song position one tick at a time.

@param beats
    the beats since the start of the song

 */
void MidiPort::sendSPP(uint16_t beats)
{
    //Error Checking:
    if(beats > 16383) beats -= 16383;
    if(beats > 16383) beats = 0;  //give up
    //Perform Data Calculation:
    uint8_t _lsb = LSB(beats);
    uint8_t _msb = MSB(beats);
    //Write Data to MIDI Port:
     byte _msg[3];
    _msg[0] = SONG_POS;
    _msg[1] = _lsb;
    _msg[2] = _msb;
    writeMsg(_msg);
  
}
/** ppor mans MTC Quarter Frame .

 */
void MidiPort::sendQuarterTimeFrame(byte msgType, byte values)
{
    //Error Checking:
    if(msgType > 7) msgType = 7;
    if(values > 15) values = 15;
    //Write Data to MIDI Port:
    _SerialObj.write((uint8_t)Q_TIME_FRAME);
    _SerialObj.write((uint8_t)(msgType << 4) + values);
}
/** MTCTimeframe Message utility.

When the time is running continuously, the 32-bit time code is broken into 8 4-bit pieces,
and one piece is transmitted each quarter frame. I.e. 96—120 times per second,
depending on the frame rate. Since it takes eight quarter frames for a complete time code message,
the complete SMPTE time is updated every two frames.

which of these 8 4-bit pieces will be sent is detemined by the param counter

A quarter-frame messages consists of a status byte of 0xF1, followed by a single 7-bit data value: 3 bits to identify the piece,
and 4 bits of partial time code.
When time is running forward, the piece numbers increment from 0–7;
with the time that piece 0 is transmitted is the coded instant, and the remaining pieces are transmitted later.


Unlike standard SMPTE timecode, MIDI timecode's quarter-frame and full-frame messages carry a two-bit flag value
that identifies the rate of the timecode, specifying it as either:

24 frame/s (standard rate for film work)
25 frame/s (standard rate for PAL video)
29.97 frame/s (drop-frame timecode for NTSC video)
30 frame/s (non-drop timecode for NTSC video)

the FPS are encoded as 24,25,30 and 29 for 29.97

@param counter [0 .. 7]
    MTC Messages are sent in 8 pieces...

@param hh [0 .. 127]
    Hour
@param mm [0 .. 59]
    mm
@param ss [0 .. 59]
    Seconds
@param ff [0 .. fps]
    Frames

@param fps [24,25,29,30]
    the FPS of the MTC Session, 29.97 is encoded as 29

 */

void MidiPort::sendMTCTimeFrame(uint8_t counter , byte hh, byte mm, byte ss, byte ff, MTCFrames fps )
{
    byte _msg[2];
    uint8_t toSend;
    switch (counter)
    {
    case 0:
        toSend = 0x00 | LO_NIBBLE(ff);
        break;
    case 1:
        toSend = 0x10 | HI_NIBBLE(ff);
        break;
    case 2:
        toSend = 0x20 | LO_NIBBLE(ss);
        break;
    case 3:
        toSend = 0x30 | HI_NIBBLE(ss);
        break;
    case 4:
        toSend = 0x40 | LO_NIBBLE(mm);
        break;
    case 5:
        toSend = 0x50 | HI_NIBBLE(mm);
        break;
    case 6:
        toSend = 0x60 | LO_NIBBLE(hh);
        break;
    case 7:
        toSend = 0x70 | HI_NIBBLE(hh);
        switch (fps)
        {
        case 24:
            toSend | 0b01;
            break;
        case 25:
            toSend | 0b01;
            break;
        case 29:
            toSend | 0b10;
            break;
        case 30:
            toSend | 0b11;
            break;
        }
    }
    _msg[0] = 0xF1;
    _msg[1] = toSend;
    writeMsg(_msg, 2);
};


