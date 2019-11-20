
/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"


using namespace nmidi;

/**
Send any type of message, except SysEx.

Parameters: cmd (CommandType), channel (Channel), arg1 (0-127), arg2 (0-127)
 */
boolean MidiPort::sendMidiEvent(CommandType cmd, Channel channel, byte arg1, byte arg2)
{
    //Error Checking:
    if(cmd < NOTE_OFF || cmd > SOFT_RESET) return false;
    if(cmd == CHANNEL_MODE) cmd = CTRL_CHANGE;
    else if(cmd == SYSEX_START || cmd == SYSEX_END) return false;
    if(arg1 > 127) arg1 = 127;
    if(arg2 > 127) arg2 = 127;
    //Write Data to MIDI Port:
    char len = getEventDataLength(cmd);
    if(cmd > 0xF0) _SerialObj.write((uint8_t)cmd);
    else _SerialObj.write((uint8_t)cmd + channel);
    if(len > 0) _SerialObj.write((uint8_t)arg1);
    if(len > 1) _SerialObj.write((uint8_t)arg2);
    return true;
}

/**
Send any type of message, except SysEx.

Parameters: cmd (CommandType), channel (Channel), arg1 (0-127), arg2 (0-127)
 */

boolean MidiPort::sendMidiEventRaw(byte cmd, byte arg1, byte arg2, int8_t dataBytes)
{
    //Error Checking:
    if(cmd < NOTE_OFF || cmd > SOFT_RESET + 0x0F) return false;
    if(cmd == SYSEX_START || cmd == SYSEX_END) return false;
    if(arg1 > 127) arg1 = 127;
    if(arg2 > 127) arg2 = 127;
    //Write Data to MIDI Port:

    _SerialObj.write((uint8_t)cmd);
    if(dataBytes > 0) _SerialObj.write((uint8_t)arg1);
    if(dataBytes > 1) _SerialObj.write((uint8_t)arg2);
    return true;
}

/** Bank Select in one message.

The Bank Select messages are actually MIDI Controller messages,
just like Volume, Pan, Sustain Pedal, Wind, and other controllers.
Specifically, the controller number for the "Most Significant Byte" (ie, MSB) of Bank Select is controller 0.
The controller number for "Least Significant Byte" (ie, LSB) of Bank Select is controller 32.
The data for these messages are the bank number you want to select.
(Sometimes the MSB Bank Select is referred to as the coarse adjustment for bank,
and the LSB Bank Select is referred to as the fine adjustment).

TEST OK

@param ch [0 .. 15]
    the Channel 0-15

@param _cc0 [0 .. 127]
    the cc0 MSB part of the Message

@param _cc32 [0 .. 127]
    the cc32 LSB part of the Message

 */
void MidiPort::sendBankChange(Channel ch , byte _cc0, byte _cc32)
{
    byte _msg[6];
    //MSB
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x00;
    _msg[2] = _cc32;
    //LSB
    _msg[4] = CTRL_CHANGE + ch;
    _msg[5] = 0x20;
    _msg[6] = _cc0;
    writeMsg(_msg, 6);
};

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

void MidiPort::sendMTCTimeFrame(uint8_t counter , byte hh, byte mm, byte ss, byte ff, byte fps )
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
/** Function 0x78

TEST OK

Mutes all sounding notes that were turned on by received Note On messages,
and which haven't yet been turned off by respective Note Off messages.
This message is not supposed to mute any notes that the musician is playing on the local keyboard.

@param ch [0 .. 15]
    the Channel 0-15


 */
void MidiPort::sendAllSoundsOff(Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x78;
    _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x79

test ok

A MIDI event that carries the MIDI reset message tells a MIDI device to reset itself.
Different MIDI devices will interpret this message differently,
but most often this message will cause a device to stop playback,
including playback set for later, set the song position to the beginning, and so on.

@param ch [0 .. 15]
    the Channel 0-15


 */
void MidiPort::sendResetAll( Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x79;
    _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x7a

test onOff

A MIDI Controller message that controls the ability of a synthesizer to be played by means of its own keyboard.
The local control message has one parameter, which may be an "on" or "off" value.
When a synth with a keyboard receives a local control off message, the sound-producing part of the synth,
and the keyboard and ancillary performance controls (pitch wheel etc.) behave as two separate units.
Pressing a key will not produce a sound but will send messages out of the MIDI out port,
while the sound-producing part of the synth will produce sound only in response to messages coming in through the MIDI in port.
Receiving a local control on message restores normal operation.

@param ch [0 .. 15]
    the Channel 0-15
@param onOff [0 = off, 64 = on]
    on or off

 */
void MidiPort::sendLocalControl(Channel ch , boolean onOff)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7a;
    if (onOff) _msg[2] = 0x40;
    else _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x7b

test ok

Turns off all notes that were turned on by received Note On messages,
and which haven't yet been turned off by respective Note Off messages.
This message is not supposed to turn off any notes that the musician is playing on the local keyboard.

@param ch [0 .. 15]
    the Channel 0-15



 */
void MidiPort::sendAllNotesOff(Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7b;
    _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x7c

test ok

The receiver should respond only to Channel Voice messages which is receives on it's Basic Channel

@param ch [0 .. 15]
    the Channel 0-15
    param


 */
void MidiPort::sendOmniOff(Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7c;
    _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x7d

test ok

The receiver should respond to Channel Voice messages which is receives on any MIDI channel

@param ch [0 .. 15]
    the Channel 0-15
@return
	The test results

 */
void MidiPort::sendOmniOn(Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7d;
    _msg[2] = 0x00;
    writeMsg(_msg);
};
/** Function 0x7e

tost ok, check voices

Puts the receiver into monophonic mode.

@param voices
    cout of voices to be allocated

@param ch [0 .. 15]
    the Channel 0-15


 */
void MidiPort::sendMonoOn(Channel ch, uint8_t voices)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7e;
    _msg[2] = voices;
    writeMsg(_msg);
};
/** Function 0x7f

test OK

Puts the receiver into polyphonic mode

@param ch
    the Midi Channel (0..15)

 */
void MidiPort::sendPolyOn(Channel ch)
{
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = 0x7f;
    _msg[2] = 0x00;
    writeMsg(_msg);
};

/** Function

Some master device that controls sequence playback sends this message to force a slave device to set a certain song for playback.

test ok

@param song
    param


 */
void MidiPort::sendSongSelect(byte song)
{
    //Error Checking:
    if(song > 0x7f) song = 0x7f;
    //Write Data to MIDI Port:
    byte _msg[2];
    _msg[0] = SONG_SELECT;
    _msg[1] = song;
    writeMsg(_msg, 2);
}
//Forces Analog Synths to Re-Tune:
/** Function

test ok
A MIDI event that carries the MIDI tune request message tells a MIDI device to tune itself.


 */
void MidiPort::sendTuneRequest()
{
    _SerialObj.write((uint8_t)TUNE_REQ);
}

/** Function

Most commonly referred to as a (CC),
this message consist of a controller number and a value ranging from 0-127.
Most hardware encoders, such as knobs and faders, will send these types of messages.
Often encoders will have the ability to change the control change number they are sending.
This flexibility allows a single encoder to have the ability to control multiple parameters such as volume, filter cutoff, effect mix, etc.

test ok

@param controlNum
    param
@param value
    value 0..127
@param ch
    the Midi Channel (0..15)

 */
void MidiPort::sendControlChange(Channel ch, byte controlNum, byte value)
{
    //Error Checking:
    // if(ch > CH16) ch = CH16;
    if(controlNum > 119) controlNum = 119;
    if(value > 127) value = 127;
    //Perform Command Calculation:
    byte _msg[3];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = controlNum;
    _msg[2] = value;
    writeMsg(_msg);

}
/** Function

14 bit version of a given CC

retest

@param controlNum
    param
@param value
    value 0..2^14
@param ch
    the Midi Channel (0..15)


 */
void  MidiPort::sendControlChangeHi(Channel ch, byte controlNum, int16_t value)
{
    // if(ch > CH16) ch = CH16;
    if(controlNum > 20) controlNum = controlNum - 20;
    //if(value > 127) value = 127;
    //Write Data to MIDI Port:
    byte _msg[6];
    _msg[0] = CTRL_CHANGE + ch;
    _msg[1] = controlNum;
    _msg[2] = MSB(value);
    _msg[3] = CTRL_CHANGE + ch ;
    _msg[4] = controlNum + 0x20;
    _msg[5] = LSB(value);
    writeMsg(_msg, 6);
}


/** Function

test ok
The Program Change message is used to specify the type of instrument
which should be used to play sounds on a given Channel.
This message needs only one data byte which specifies the new program number.

@param program
    param
@param ch
    the Midi Channel (0..15)


 */
void MidiPort::sendProgramChange(Channel ch, byte program)
{
    //Error Checking:
    // if(ch > CH16) ch = CH16;
    if(program > 127) program = 127;
    //Perform Command Calculation:
    byte _msg[3];
    _msg[0] = PROG_CHANGE + ch;
    _msg[1] = program;
    _msg[2] = 0x00;
    writeMsg(_msg);
}


/** Function

Basically the same as sendKeyPressure, but affects all keys at once:

@param pressure
    param
@param ch
    the Midi Channel (0..15)


 */
void MidiPort::sendChannelPressure(Channel ch, byte pressure)
{
    //Error Checking:
    // if(ch > CH16) ch = CH16;
    if(pressure > 127) pressure = 127;
    //Perform Command Calculation:
    //Write Data to MIDI Port:

    byte _msg[3];
    _msg[0] = CHANNEL_AT + ch;
    _msg[1] = pressure;
    _msg[2] = 0x00;
    writeMsg(_msg);
}

/** sendPitchBendCenter

The MIDI protocol specifies that a pitchbend value of 8192 (MSB of 64 and LSB of 0) means no bend.

@param ch
    the Midi Channel (0..15)
 */
void MidiPort::sendPitchBendCenter(Channel ch)
{
    sendPitchBend(ch, 0x2000);
}


/** sendPitchBend

The MIDI protocol specifies that a pitchbend value of 8192 (MSB of 64 and LSB of 0) means no bend.
Thus, on the scale from 0 to 16,383, a value of 0 means maximum downward bend,
8,192 means no bend, and 16,383 means maximum upward bend.
Almost all pitchbend wheels on MIDI controllers use a spring mechanism that has the dual function of
a) providing tactile resistance feedback as one moves the wheel away from its centered position and
b) snapping the wheel quickly back to its centered position when it’s not being manipulated.

@param ch
    the Midi Channel (0..15)
@param value
    0 to 16,383, with 8192 as CENTER


 */
void MidiPort::sendPitchBend(Channel ch, int16_t value)
{
    //Error Checking:
    // if(ch > CH16) ch = CH16;
    if(value > 8191) value = 8191;
    if(value < -8192) value = -8192;
    //Perform Command Calculation:

    //Convert signed 14 bit to un-signed 14 bit:
    uint16_t uvalue = value + 8192;
    //This is a 14 bit number, and 8 is half of 16, so
    //we'll clear out the un-used 8th bit with a binary AND:
    uint8_t lessSigBits = uvalue & 0b01111111;
    //As for the most significant 7 bits, we'll just do some bit-shifting:
    uint8_t moreSigBits = uvalue >> 7;
    //Write Data to MIDI Port:
    byte _msg[3];
    _msg[0] = PITCH_BEND + ch;
    _msg[1] = lessSigBits;
    _msg[2] = moreSigBits;
    writeMsg(_msg);
}


/** activeSense

Just don't use it. It's pointless.

 */
void MidiPort::activeSense()
{
    writeMsg(ACTIVE_SENSE);

}

//Send if the velocity changes while holding down the note:
//Parameters: channel (Channel), key (0-127), pressure (0-127)

/**
 *  \brief KeyPressure Poly
 *
 *  \param [in] ch Channel
 *  \param [in] key note
 *  \param [in] pressure 0-127
 */
void MidiPort::sendKeyPressure(Channel ch, byte key, byte pressure)
{
    //Error Checking:
    if(ch > CH16) ch = CH16;
    if(key > 127) key = 127;
    if(pressure > 127) pressure = 127;
    //Perform Command Calculation:
    byte _msg[3];
    _msg[0] = POLY_AT + ch;
    _msg[1] = key;
    _msg[2] = pressure;
    writeMsg(_msg);
}
