
/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"
#define EXPERIMENTAL

using namespace nmidi;

//----- Event Handler Firing Code -----
/** Function

Text


 */
void MidiPort::scanForData()
{
	CommandType event = readPort();

	//Trigger Events:
	if(event != NO_NEW_MSG && event != INVALID) {

		if(flt_channel()) { //If on correct channel:
			if(event == NOTE_ON)  _keysPressed++;
			if(event == NOTE_OFF) _keysPressed--;
			if (_keysPressed > 20) _keysPressed = 0;


			if(_fptrMidiEvent != 0) {
				boolean result = _fptrMidiEvent(_portID, event, msg_channel,msg_length, _msg);
				if (!result) return;
			}
			// interface
#ifdef USE_NMIDI_INTERFACES
			if(pMidiEventHandler != 0) {
				boolean result = pMidiEventHandler->_handleMidiEvent(_portID, event, msg_channel, msg_length,_msg);
				if (!result) return;
			}
#endif //USE_NMIDI_INTERFACES			
			//Voice Messages:
			if     (event == NOTE_ON && _fptrNoteOn) _fptrNoteOn(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == NOTE_OFF && _fptrNoteOff) _fptrNoteOff(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == POLY_AT && _fptrKeyPressure) _fptrKeyPressure(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == CTRL_CHANGE && _fptrControlChange) _fptrControlChange(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == PROG_CHANGE && _fptrProgramChange) _fptrProgramChange(_portID, msg_channel, msg_param1);
			else if(event == CHANNEL_AT && _fptrChannelPressure) _fptrChannelPressure(_portID, msg_channel, msg_param1);
			else if(event == PITCH_BEND && _fptrPitchBend) _fptrPitchBend(_portID, msg_channel, ((msg_param2 << 7) + msg_param1 - 8192));
			//Mode Messages:
			else if(event == CHANNEL_MODE && _fptrChannelMode) _fptrChannelMode(_portID, msg_channel, msg_param1, msg_param2);
			//System Messages:
			else if(event == SYSEX_START && _fptrSysEx && msg_length > 1) 
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
				 _fptrSysEx(_portID,idCode, idTrim, _rawData, msg_length);
				*/
				//_fptrSysEx(_portID,nullptr,nullptr,nullptr,0);
			} 
			else if(event == Q_TIME_FRAME && _fptrQuarterFrame) _fptrQuarterFrame(_portID, (msg_param2 >> 4), (msg_param1 & 0x0F));
			else if(event == SONG_POS && _fptrSongPosition) _fptrSongPosition(_portID, (msg_param2 << 7) + msg_param1);
			else if(event == SONG_SELECT && _fptrSongSelect) _fptrSongSelect(_portID, msg_param1);
			else if(event == TUNE_REQ && _fptrTuneRequest) _fptrTuneRequest(_portID);
			//Real-Time Messages:
			else if(event == TIMING_CLOCK && _fptrTimingClock) _fptrTimingClock(_portID);
			else if(event == START && _fptrStart) _fptrStart(_portID);
			else if(event == CONTINUE && _fptrContinue) _fptrContinue(_portID);
			else if(event == STOP && _fptrStop) _fptrStop(_portID);
			else if(event == ACTIVE_SENSE && _fptrActiveSense) _fptrActiveSense(_portID);
			else if(event == SOFT_RESET && _fptrReset) _fptrReset(_portID);
			//Give microcontroller some time to think:
			//delayMicroseconds(1);
			// interfaces
#ifdef USE_NMIDI_INTERFACES
			//Voice Messages:
			if     (event == NOTE_ON && pNoteOnHandler) pNoteOnHandler->_handleNoteOn(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == NOTE_OFF && pNoteOffHandler) pNoteOffHandler->_handleNoteOff(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == POLY_AT && pKeyPressureHandler) pKeyPressureHandler->_handleKeyPressure(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == CTRL_CHANGE && pControlChangeHandler) pControlChangeHandler->_handleControlChange(_portID, msg_channel, msg_param1, msg_param2);
			else if(event == PROG_CHANGE && pProgramChangeHandler) pProgramChangeHandler->_handleProgramChange(_portID, msg_channel, msg_param1);
			else if(event == CHANNEL_AT && pChannelPressureHandler) pChannelPressureHandler->_handleChannelPressure(_portID, msg_channel, msg_param1);
			else if(event == PITCH_BEND && pPitchBendHandler) pPitchBendHandler->_handlePitchBend(_portID, msg_channel, ((msg_param2 << 7) + msg_param1 - 8192));
			//Mode Messages:
			else if(event == CHANNEL_MODE && pChannelModeHandler) pChannelModeHandler->_handleChannelMode(_portID, msg_channel, msg_param1, msg_param2);
			//System Messages:
			else if(event == SYSEX_START && pSysExHandler && msg_length > 1) {
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
			} else if(event == Q_TIME_FRAME && pQuarterFrameHandler) pQuarterFrameHandler->_handleQuarterFrame(_portID, (msg_param2 >> 4), (msg_param1 & 0x0F));
			else if(event == SONG_POS && pSongPositionHandler) pSongPositionHandler->_handleSongPosition(_portID, (msg_param2 << 7) + msg_param1);
			else if(event == SONG_SELECT && pSongSelectHandler) pSongSelectHandler->_handleSongSelect(_portID, msg_param1);
			else if(event == TUNE_REQ && pTuneRequestHandler) pTuneRequestHandler->_handleTuneRequest(_portID);
			//Real-Time Messages:
			else if(event == TIMING_CLOCK && pTimingClockHandler) pTimingClockHandler->_handleTimingClock(_portID);
			else if(event == START && pStartHandler) pStartHandler->_handleStart(_portID);
			else if(event == CONTINUE && pContinueHandler) pContinueHandler->_handleContinue(_portID);
			else if(event == STOP && pStopHandler) pStopHandler->_handleStop(_portID);
			else if(event == ACTIVE_SENSE && pActiveSenseHandler) pActiveSenseHandler->_handleActiveSense(_portID); // pfui bah
			else if(event == SOFT_RESET && pResetHandler) pResetHandler->_handleReset(_portID);
#endif //USE_NMIDI_INTERFACES
		}
		//Reset Vars:
		if(msg_length) {
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
	while(_SerialObjIn->available() > 0) {
#ifdef EXPERIMENTAL
		noInterrupts();
#endif
		//Read Next Byte:
		byte newByte = _SerialObjIn->read();
		
		if(_thruMode == FORWARD_ALL) _SerialObjOut->write((uint8_t)newByte);
		if(newByte > 0x7f) {  // new Message

			//TODO: Clock may interrupt command
			if (msg_discard) msg_discard = false;
			//todo: incomplete Messages
			msg_status = newByte;
			msg_command = getEventCmdType(newByte);
			if (isChannelMessage(msg_command)) {
				msg_channel = (Channel)(newByte & 0x0F);
			} else {
				msg_channel = CH_NONE;
			}
			// msg_pending_bytes is Variable when sysex
			msg_pending_bytes = getEventDataLength(msg_command);
			msg_length = msg_pending_bytes + 1;

		} 
		else 
		{
			if (msg_discard) 
			{
#ifdef EXPERIMENTAL
				interrupts();
#endif
				return INVALID;
			}

			if (msg_pending_bytes == 0) { //running status message
				if (isChannelMessage(msg_command)) {
					msg_pending_bytes = getEventDataLength(msg_command);
				} else {
					msg_discard = true;
#ifdef EXPERIMENTAL
					interrupts();
#endif
					return INVALID;
				}
			}

			if ((msg_length - msg_pending_bytes) == 1) {
				msg_param1 = newByte;
			}
			if ((msg_length - msg_pending_bytes) == 2) {
				msg_param2 = newByte;
			}
			msg_pending_bytes--;
		}
		if (msg_pending_bytes == 0) {   //Message completely parsed , no more Params
#ifdef EXPERIMENTAL
			interrupts();
#endif
			return msg_command;
		}
	}
#ifdef EXPERIMENTAL
	interrupts();
#endif
	return NO_NEW_MSG;
};
void MidiPort::internalClockEvent(byte b)
{
#ifdef EXPERIMENTAL
	noInterrupts();
#endif
	
	switch(b) {
	case 0XF0: { //CLK
		byte c[] = {(byte)0XF0};
		if(_fptrTimingClock) _fptrTimingClock(_portID);
		if(_fptrMidiEvent != 0) {
			_fptrMidiEvent(_portID, TIMING_CLOCK, CH_ALL,1, c);
		};
		break;
	}
	case 0xFA: { //start
		byte c[] = {(byte)0XFA};
		if(_fptrStart) _fptrStart(_portID);
		if(_fptrMidiEvent != 0) {
			_fptrMidiEvent(_portID, START, CH_ALL,1, c);
		};
		break;
	}
	case 0xFB: { //Continue
		byte c[] = {(byte)0XFB};
		if(_fptrContinue) _fptrContinue(_portID);
		if(_fptrMidiEvent != 0) {
			_fptrMidiEvent(_portID, CONTINUE, CH_ALL, 1, c);
		};
		break;
	}
	case 0xFC: { //stop
		byte c[] = {(byte)0XFC};
		if(_fptrStop) _fptrStop(_portID);
		if(_fptrMidiEvent != 0) {
			_fptrMidiEvent(_portID, STOP, CH_ALL, 1, c);
		};
		break;
	}
	}



#ifdef EXPERIMENTAL
	interrupts();
#endif

}