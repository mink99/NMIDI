
/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"


using namespace nmidi;

//----- Event Handler Firing Code -----
/** Function

Text


 */
void MidiPort::scanForData()
{
    CommandType event = readPort();
    
    //Trigger Events:
    if(event != NO_NEW_MSG && event != INVALID)
    {
        if(flt_channel()) //If on correct channel:
        {
            if(event == NOTE_ON)  _keysPressed++;
            if(event == NOTE_OFF) _keysPressed--;
            if (_keysPressed > 20) _keysPressed = 0;


            if(_handleMidiEvent != 0)
            {
                boolean result = _handleMidiEvent(_portID, event, _activeChannel, _msg, msg_length);
                if (!result) return;
            }
            //Voice Messages:
            if     (event == NOTE_ON && _handleNoteOn) _handleNoteOn(_portID, _activeChannel, msg_param1, msg_param2);
            else if(event == NOTE_OFF && _handleNoteOff) _handleNoteOff(_portID, _activeChannel, msg_param1, msg_param2);
            else if(event == POLY_AT && _handleKeyPressure) _handleKeyPressure(_portID, _activeChannel, msg_param1, msg_param2);
            else if(event == CTRL_CHANGE && _handleControlChange) _handleControlChange(_portID, _activeChannel, msg_param1, msg_param2);
            else if(event == PROG_CHANGE && _handleProgramChange) _handleProgramChange(_portID, _activeChannel, msg_param1);
            else if(event == CHANNEL_AT && _handleChannelPressure) _handleChannelPressure(_portID, _activeChannel, msg_param1);
            else if(event == PITCH_BEND && _handlePitchBend) _handlePitchBend(_portID, _activeChannel, ((msg_param2 << 7) + msg_param1 - 8192));
            //Mode Messages:
            else if(event == CHANNEL_MODE && _handleChannelMode) _handleChannelMode(_portID, _activeChannel, msg_param1, msg_param2);
            //System Messages:
            else if(event == SYSEX_START && _handleSysEx && msg_length > 1)
            {
                //Determine ID Code Length:
                /*
                 byte idTrim;
                 byte idCode[3] = {};
                 if(_rawData[0] == 0 && msg_length > 3)
                 {
                     idTrim = 3;
                 }
                 else
                 {
                     idTrim = 1;
                 }
                 //Strip ID Code off of SysEx Data:
                 for(byte r = 0; r < idTrim; r++)
                 {
                     idCode[r] = _rawData[0];
                     for(byte i = 0; i < msg_length - 1; i++)
                     {
                         _rawData[i] = _rawData[i + 1];
                     }
                     msg_length--;
                     _rawData[msg_length] = 0;
                 }
                 //Trigger Handler:
                 _handleSysEx(_portID,idCode, idTrim, _rawData, msg_length);
                */
            }
            else if(event == Q_TIME_FRAME && _handleQuarterFrame) _handleQuarterFrame(_portID, (msg_param2 >> 4), (msg_param1 & 0x0F));
            else if(event == SONG_POS && _handleSongPosition) _handleSongPosition(_portID, (msg_param2 << 7) + msg_param1);
            else if(event == SONG_SELECT && _handleSongSelect) _handleSongSelect(_portID, msg_param1);
            else if(event == TUNE_REQ && _handleTuneRequest) _handleTuneRequest(_portID);
            //Real-Time Messages:
            else if(event == TIMING_CLOCK && _handleTimingClock) _handleTimingClock(_portID);
            else if(event == START && _handleStart) _handleStart(_portID);
            else if(event == CONTINUE && _handleContinue) _handleContinue(_portID);
            else if(event == STOP && _handleStop) _handleStop(_portID);
            else if(event == ACTIVE_SENSE && _handleActiveSense) _handleActiveSense(_portID);
            else if(event == SOFT_RESET && _handleReset) _handleReset(_portID);
            //Give microcontroller some time to think:
            //delayMicroseconds(1);
        }
        //Reset Vars:
        if(msg_length)
        {
            memset(_rawData, 0, msg_length * 2);
            msg_length = 0;
        }
        _previousCmdType = NO_NEW_MSG;
    }
}
//Central function to check for and process MIDI data:
/** Function

Text


@return
	The test results

 */
CommandType MidiPort::readPort()
{
    //Read Available MIDI Data:
    while(_SerialObjIn->available() > 0)
    {
        //Read Next Byte:
        byte newByte = _SerialObjIn->read();
        if(_thruMode == FORWARD_ALL) _SerialObjOut->write((uint8_t)newByte);
        if(newByte > 0x7f)    // new Message
        {
            
            //TODO: Clock may interrupt command
            if (msg_discard) msg_discard = false;
            //todo: incomplete Messages
            msg_status = newByte;
            msg_command = getEventCmdType(newByte);
            
            if (isChannelMessage(msg_command))
            {
                msg_channel = (Channel)(newByte & 0x0F);
            }
            else
            {
                msg_channel = CH_NONE;
            }
            
            msg_pending_bytes = getEventDataLength(msg_command);
            msg_length = msg_pending_bytes + 1;
            
        }
        else
        {
            if (msg_discard) return INVALID;
            
            if (msg_pending_bytes == 0)  //running status message
            {
                if (isChannelMessage(msg_command))
                {
                    msg_pending_bytes = getEventDataLength(msg_command);
                }
                else
                {
                    msg_discard = true;
                    return INVALID;
                }
            }
            
            if ((msg_length - msg_pending_bytes) == 1)
            {
                msg_param1 = newByte;
            }
            if ((msg_length - msg_pending_bytes) == 2)
            {
                msg_param2 = newByte;
            }
            msg_pending_bytes--;
        }
        if (!msg_pending_bytes) return msg_command;
        
    }
	return NO_NEW_MSG;
};