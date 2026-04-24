#include <Stream.h>
#include <Arduino.h>

#include "NMidi.h"


using namespace nmidi;

bool msgProcessing_Sysex = false;
bool msgProcessing_Channel = false;
bool msgProcessing_System = false;
bool msgProcessing_DropMessages = false;

#ifndef RECEIVEBUFFERSIZE
#define RECEIVEBUFFERSIZE 64
#endif

int8_t channelMessageByteCount = 0;
int8_t receiveCount = 0;
const int8_t receiveBuffersize = RECEIVEBUFFERSIZE;
byte receiveBuffer[receiveBuffersize];




void MidiPort::scanForDataA()  //alternative Parser
{
	while(_SerialObjIn->available() > 0)
	{
		byte newByte = _SerialObjIn->read();
		//- Forward
		if(_thruMode == FORWARD_ALL) _SerialObjOut->write((uint8_t)newByte);

		if(newByte > 0x7f)  // Is Command
		{
			msgProcessing_DropMessages = false;
			msg_command = getEventCmdType(newByte);
			msg_channel = getEventChannel(newByte);
			//-->> apply Channel Filter ----------------------------------------------
			if (!isRegisteredChannel())
			{
				msgProcessing_DropMessages = true;
				continue;
			}
			//-->> apply NoteCounter -------------------------------------------------
			if(msg_command == NOTE_ON)  _keysPressed++;
			if(msg_command == NOTE_OFF) _keysPressed--;
			if (_keysPressed > 20) _keysPressed = 0;
			if (_keysPressed < 0) _keysPressed = 0;

			//if(!flt_channel())  continue; //If on correct channel: -----------------
			if(_fptrMidiEvent != 0)
			{
				boolean result = _fptrMidiEvent(_portID, msg_command, msg_channel, 0, nullptr);
				if (!result) continue;
			}
			//-->> apply Command Registry
			// --> ünly commands with Callback are processed
			if (!isRegisteredCommand(msg_command))
			{
				msgProcessing_DropMessages = true;
				continue;
			}
			if (msg_command == SYSEX_START && _fptrSysEx)
			{
				msgProcessing_DropMessages = false;
				msgProcessing_Sysex = true;
				receiveCount = 0;
				memset(receiveBuffer, 0, receiveBuffersize);
				continue;
			}
			if (isRealtimeMessage(msg_command))
			{
				msgProcessing_DropMessages = false;
				if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, 0, nullptr);
				if(msg_command == START && _fptrStart) _fptrStart(_portID);
				else if(msg_command == CONTINUE && _fptrContinue) _fptrContinue(_portID);
				else if(msg_command == STOP && _fptrStop) _fptrStop(_portID);
				else if(msg_command == TIMING_CLOCK && _fptrTimingClock) _fptrTimingClock(_portID);
				continue;
			}
			if (isChannelMessage(msg_command))
			{
				msgProcessing_Channel = true;
				msgProcessing_DropMessages = false;
				channelMessageByteCount = 0;
				receiveBuffer[0] = 0x0;
				receiveBuffer[1] = 0x0;
				continue;
			}
			if (isSystemMessage(msg_command))
			{
				msgProcessing_System = true;
				msgProcessing_DropMessages = false;
				channelMessageByteCount = 0;
				receiveBuffer[0] = 0x0;
				receiveBuffer[1] = 0x0;
				if(msg_command == TUNE_REQ && _fptrTuneRequest) _fptrTuneRequest(_portID);
				continue;
			}
		}
		else // is Data
		{
			if (msgProcessing_DropMessages) continue;
			if (msgProcessing_Sysex)
			{
				receiveBuffer[receiveCount] = newByte;
				receiveCount++;
				// check overflow
				if (newByte == SYSEX_END)
				{
					if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, receiveBuffersize, receiveBuffer);
					if(_fptrSysEx) _fptrSysEx(_portID,receiveBuffersize,receiveBuffer);
					msgProcessing_Sysex = false;
					receiveCount = 0;
					memset(receiveBuffer, 0, receiveBuffersize);
				}
			}
			if (msgProcessing_Channel) // always 2 Bytes
			{
				receiveBuffer[channelMessageByteCount] = newByte;
				channelMessageByteCount++;
				if (channelMessageByteCount == 1)
				{

					switch(msg_command)
					{
					case CHANNEL_AT:
						if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, 1, receiveBuffer);
						if (_fptrChannelPressure) _fptrChannelPressure(_portID, msg_channel, receiveBuffer[0]);
						break;
					case PROG_CHANGE:
						if (_fptrProgramChange) _fptrProgramChange(_portID, msg_channel, receiveBuffer[0]);
						break;
					}
				}
				if (channelMessageByteCount == 2)
				{
					if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, 2, receiveBuffer);
					switch(msg_command)
					{
					case NOTE_OFF:
						if (_fptrNoteOff) _fptrNoteOff(_portID, msg_channel, receiveBuffer[0], receiveBuffer[1]);
						break;
					case NOTE_ON:
						if (_fptrNoteOn) _fptrNoteOn(_portID, msg_channel, receiveBuffer[0], receiveBuffer[1]);
						break;
					case CTRL_CHANGE:
						if (_fptrControlChange) _fptrControlChange(_portID, msg_channel, receiveBuffer[0], receiveBuffer[1]);
						break;
					case CHANNEL_MODE:
						if (_fptrChannelMode) _fptrChannelMode(_portID, msg_channel, receiveBuffer[0], receiveBuffer[1]);
						break;
					case PITCH_BEND:
						if (_fptrPitchBend) _fptrPitchBend(_portID, msg_channel, ((receiveBuffer[1] << 7) + receiveBuffer[0] - 8192));
						break;
					case POLY_AT:
						if (_fptrKeyPressure) _fptrKeyPressure(_portID, msg_channel, receiveBuffer[0], receiveBuffer[1]);
						break;
					}
				}
			}
			if (msgProcessing_System)
			{
				/*
					Q_TIME_FRAME     = 0xF1, ///< Qarter Time Frame
					SONG_POS         = 0xF2, ///< SPP
					SONG_SELECT      = 0xF3, ///< Does something or the other...
					TUNE_REQ         = 0xF6, ///< Tune Your Insturments!

				*/
				if (channelMessageByteCount == 1)
				{

					switch(msg_command)
					{
					case SONG_SELECT:
						if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, 1, receiveBuffer);
						if (_fptrSongSelect) _fptrSongSelect(_portID, receiveBuffer[0]);
						break;
					}
				}
				if (channelMessageByteCount == 2)
				{

					switch(msg_command)
					{
					case SONG_POS:
						if(_fptrMidiEvent)_fptrMidiEvent(_portID, msg_command, msg_channel, 2, receiveBuffer);
						if (_fptrSongPosition) _fptrSongPosition(_portID, (receiveBuffer[1] << 7) + receiveBuffer[0]);
						break;
					}
				}
			}
		}
	}
}
bool MidiPort::isRegisteredChannel()
{
	//if(msg_channel == 0) return true; //<-- If listen to all channels. vvv If listen to one channel.
	if(_listenCh == CH_ALL) return true; //<-- If listen to all channels. vvv If listen to one channel.
	else if(msg_channel == _listenCh || msg_channel == CH_ALL || msg_channel == CH_NONE) return true; //If on same channel or no channel.
	else return false;
}
bool MidiPort::isRegisteredCommand(CommandType cmd)
{
	// if handler is Registered....
	// msg_command in Registry
	if (callbackRegistry.count(cmd) > 0)
	{
		return true;
	}
	return false;
};
void MidiPort::registerCommand(CommandType cmd)
{
	if (callbackRegistry.count(cmd) > 0) return ;
	callbackRegistry.insert(cmd);

};
void MidiPort::unregisterCommand(CommandType cmd)
{
	if (callbackRegistry.count(cmd) > 0)
	{
		callbackRegistry.erase(cmd);
	}

};
