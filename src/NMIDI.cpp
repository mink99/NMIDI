
/** \mainpage MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu

NMidi Libary C++ FILE v2.1 by Mink

\section basic_sec Basic Concepts
\subsection step1 callbacks
\subsection step2 multi instance
\subsection step3 portable

\section reading_sec Reading and Processing

Declaring a MidiPort Instance

\code
  MidiPort midi = MidiPort(Serial1);
\endcode

Instanciating a MidiPort Instance

\code
  Serial1.begin(31250);
  midi.begin();

  midi.setThruMode(FORWARD_ALL);
  midi.handleMidiEvent(processData);
\endcode

Integrating a MidiPort Instance

\code
	void serialEvent()
	{
		midi.scanForData();
	}
\endcode
... or ...
\code
	void  loop()
	{
		midi.scanForData();
	}
\endcode

Serial Event is called when data is available. this may lead to a more smooth performance, but also to more "globals"


\subsection step5 main processing : handleMidiEvent()
\subsection step6 specific processing : handle_xxx()

\section filtering_sec Filtering Input

under construction ...

\section writing_sec Writing Midi Data

under construction ...

\subsection writing_sec2 Running Status
The MIDI spec allows for a MIDI message to be sent without its Status byte (ie, just its data bytes are sent)
as long as the previous, transmitted message had the same Status.
This is referred to as running status.
Running status is simply a clever scheme to maximize the efficiency of MIDI transmission (by removing extraneous Status bytes).
The basic philosophy of running status is that a device must always remember the last Status byte that it received (except for RealTime),
and if it doesn't receive a Status byte when expected (on subsequent messages),
it should assume that it's dealing with a running status situation.
 A device that generates MIDI messages should always remember the last Status byte that it sent (except for RealTime),
 and if it needs to send another message with the same Status, the Status byte may be omitted.

\code
	void enableRunningStatus(boolean enable = true)
\endcode

\section sysex_sec Writing SYSEX and NRPN

\subsection writing_sec3 writing sysex

writing sysex is basically three steps:
\code
void beginTransferSysex(byte,byte ); //0xf0
void transferSysex(byte[], uint8_t);
void endTransferSysex(); //0xf7
\endcode

you can repeat transferSysex multiple times in a loop for eg doing block transfer while reading from sd
you may test the timing, no precautios are taken within nmidi. this is not midi-ox :)


example :
\code
beginTransferSysex(0x41,0x10 ); //0xf0
int x = 0;

while (...)
{
	data = read(...);
	transferSysex(data, size);
}
endTransferSysex();
\endcode



\subsection writing_sec4 writing sysex the roland way

special functions are availoable for roland devices (0x41) and their checksum management

uint8_t transferSysexR(byte[], uint8_t size , uint8_t prev ); //0xf7

example :
\code
beginTransferSysex(0x41,0x10 ); //0xf0
int x = 0;

while (...)
{
	data = read(...);
	x = transferSysexR(data, size, x);
}
endTransferSysexR(x);
\endcode

while in a loop, you provide the data, but also read the result of the function ans provide it either to the nex call or to
void endTransferSysexR(byte ); //0xf7

\section session_sec Writing Midi Clock and MTC : Sessions

under construction ...

\section util_sec Utilities

under construction ...

\section dp_sec Design Patterns

\subsection dp_sec1 multiple handleMidiEvent() for status changes

\subsection dp_sec2 design considerations for multiple ports


\section poratability_sec Considerations about Portability

under construction ...


 */

#include <Stream.h>
#include <Arduino.h>
#define USE_INTERFACES
#include "NMidi.h"


using namespace nmidi;

//Main Library Functions:
MidiPort::MidiPort(Channel listenCh)
{
	if(listenCh > CH_ALL) listenCh = CH_ALL;
	_listenCh = listenCh;
	_thruMode = FORWARD_OFF;
	_runningStatus = false;

	_handleMidiEvent       = 0;
	_handleNoteOn        = 0;
	_handleNoteOff         = 0;
	_handleKeyPressure   = 0;
	_handleControlChange   = 0;
	_handleProgramChange = 0;
	_handleChannelPressure = 0;
	_handlePitchBend     = 0;
	_handleChannelMode     = 0;
	_handleSysEx         = 0;
	_handleQuarterFrame    = 0;
	_handleSongPosition  = 0;
	_handleSongSelect      = 0;
	_handleTuneRequest   = 0;
	_handleTimingClock     = 0;
	_handleStart         = 0;
	_handleContinue        = 0;
	_handleStop          = 0;
	_handleActiveSense     = 0;
	_handleReset         = 0;

#ifdef USE_NMIDI_INTERFACES
	pMidiEventHandler = 0;
	pNoteOnHandler = 0;
	pNoteOffHandler = 0;
	pKeyPressureHandler = 0;
	pControlChangeHandler = 0;
	pProgramChangeHandler = 0;
	pChannelPressureHandler = 0;
	pPitchBendHandler = 0;
	pChannelModeHandler = 0;
	pSysExHandler = 0;
	pQuarterFrameHandler = 0;
	pSongPositionHandler = 0;
	pSongSelectHandler = 0;
	pTuneRequestHandler = 0;
	pTimingClockHandler = 0;
	pStartHandler = 0;
	pContinueHandler = 0;
	pStopHandler = 0;
	pActiveSenseHandler = 0;
	pResetHandler = 0;
#endif // USE_NMIDI_INTERFACES
};

MidiPort::MidiPort(Stream &serialObjectInOut, Channel listenCh) : MidiPort::MidiPort(serialObjectInOut,serialObjectInOut,listenCh) {};


//MidiPort::MidiPort(Stream &serialObjectIn,Stream &serialObjectOut, Channel listenCh): MidiPort::MidiPort(listenCh),_SerialObjIn(serialObjectIn),_SerialObjOut(serialObjectOut)
MidiPort::MidiPort(Stream &serialObjectIn,Stream &serialObjectOut, Channel listenCh): MidiPort::MidiPort(listenCh)
{
	if(listenCh > CH_ALL) listenCh = CH_ALL;
	_listenCh = listenCh;
	_thruMode = FORWARD_OFF;
	_runningStatus = false;

	_SerialObjIn  = &serialObjectIn;
	_SerialObjOut = &serialObjectOut;


	//Make Sure Handler Pointers Are Empty:
	_handleMidiEvent       = 0;
	_handleNoteOn        = 0;
	_handleNoteOff         = 0;
	_handleKeyPressure   = 0;
	_handleControlChange   = 0;
	_handleProgramChange = 0;
	_handleChannelPressure = 0;
	_handlePitchBend     = 0;
	_handleChannelMode     = 0;
	_handleSysEx         = 0;
	_handleQuarterFrame    = 0;
	_handleSongPosition  = 0;
	_handleSongSelect      = 0;
	_handleTuneRequest   = 0;
	_handleTimingClock     = 0;
	_handleStart         = 0;
	_handleContinue        = 0;
	_handleStop          = 0;
	_handleActiveSense     = 0;
	_handleReset         = 0;

#ifdef USE_NMIDI_INTERFACES
	pMidiEventHandler = 0;
	pNoteOnHandler = 0;
	pNoteOffHandler = 0;
	pKeyPressureHandler = 0;
	pControlChangeHandler = 0;
	pProgramChangeHandler = 0;
	pChannelPressureHandler = 0;
	pPitchBendHandler = 0;
	pChannelModeHandler = 0;
	pSysExHandler = 0;
	pQuarterFrameHandler = 0;
	pSongPositionHandler = 0;
	pSongSelectHandler = 0;
	pTuneRequestHandler = 0;
	pTimingClockHandler = 0;
	pStartHandler = 0;
	pContinueHandler = 0;
	pStopHandler = 0;
	pActiveSenseHandler = 0;
	pResetHandler = 0;

#endif //USE_NMIDI_INTERFACES
}
// initialise the Library
//Parameters: id the identifier for this port, this will be provided back to the callbacks to distinguish the ports when using a callback on several ports
void MidiPort::begin(int8_t id) ///< provide an identifyer for this port
{
	_portID = id;

}
// initialise the Library
//Parameters: id the identifier for this port; this will be provided back to the callbacks to distinguish the ports when using a callback on several ports,
//            serialObjectInOut, the transportstream for input and output

void  MidiPort::begin(Stream &serialObjectInOut, int8_t id)
{
	_SerialObjIn = &serialObjectInOut;
	_SerialObjOut = &serialObjectInOut;
	_portID = id;
}
// initialise the Library
//Parameters: id the identifier for this port; this will be provided back to the callbacks to distinguish the ports when using a callback on several ports,
//            serialObjectIn, the transportstream for input
//            serialObjectOut, the transportstream for output

void  MidiPort::begin(Stream &serialObjectIn,Stream &serialObjectOut, int8_t id)
{
	_SerialObjIn = &serialObjectIn;
	_SerialObjOut = &serialObjectOut;
	_portID = id;
}

void MidiPort::setInputChannelFilter(Channel listenCh) ///< process just one channel
{
	if(listenCh > CH_ALL) listenCh = CH_ALL;
	_listenCh = listenCh;
}

void MidiPort::setThruMode(ThruMode mode)
{
	if(mode > FORWARD_ALL) mode = FORWARD_ALL;
	_thruMode = mode;
}

//---------------------------------------- Send MIDI Messages ----------------------------------------


//

//Send when a note is depressed:
//Parameters: channel (Channel), key (0-127), velocity (0-127)
void MidiPort::sendNoteOn(Channel ch, byte key, byte velocity)
{
	//Error Checking:
	if(ch > CH16) ch = CH16;
	if(key > 127) key = 127;
	if(velocity > 127) velocity = 127;
	//Perform Command Calculation:
	byte _msg[3];
	_msg[0] = NOTE_ON + ch;
	_msg[1] = key;
	_msg[2] = velocity;
	writeMsg(_msg);
};


//Send when a note is released:
//Parameters: channel (Channel), key (0-127)
void MidiPort::sendNoteOff(Channel ch, byte key, byte velocity)
{
	//Error Checking:
	if(ch > CH16) ch = CH16;
	if(key > 127) key = 127;
	//Perform Command Calculation:

	//Write Data to MIDI Port:
	byte _msg[3];
	if (_runningStatus)
	{
		_msg[0] = NOTE_ON + ch;
		_msg[1] = key;
		_msg[2] = 0;
	}
	else
	{
		_msg[0] = NOTE_OFF + ch;
		_msg[1] = key;
		_msg[2] = velocity;
	}
	writeMsg(_msg);
}






//Some special settings (take a look at the MIDI spec):
//Parameters: channel (Channel), settingCode (120-127), value (0-127)
void MidiPort::modeChange(Channel channel, byte settingCode, byte value)
{
	//Error Checking:
	if(channel > CH16) channel = CH16;
	if(settingCode > 127) settingCode = 127;
	if(settingCode < 120) settingCode = 120;
	if(value > 127) value = 127;
	//Perform Command Calculation:
	uint8_t cmd = CTRL_CHANGE + channel;
	//Write Data to MIDI Port:
	_SerialObjOut->write((uint8_t)cmd);
	_SerialObjOut->write((uint8_t)settingCode);
	_SerialObjOut->write((uint8_t)value);
}


//Send SysEx strings:
//(SysEx messages can have as many lines as you want, and each line is a 7-bit (0-127) number.)
//Parameters: idCode (1-3 length table [0-127], mfg ID code), idCodeLength (1-3), data (1+ length table [0-127]), dataLength (1+)
void MidiPort::sendSysEx(byte idCode[], unsigned int idCodeLength, byte data[], unsigned int dataLength)
{
	//Error Checking:
	if(idCodeLength > 3) idCodeLength = 3;
	if(idCodeLength == 0 || dataLength == 0) return;
	//Write Command to MIDI Port:
	_SerialObjOut->write((uint8_t)SYSEX_START);
	//Write ID Code to MIDI Port:
	for(int i = 0; i < idCodeLength; i++)
	{
		if(idCode[i] > 127) _SerialObjOut->write((uint8_t)127);
		else _SerialObjOut->write((uint8_t)idCode[i]);
	}
	//Write SysEx Data to MIDI Port:
	for(int i = 0; i < dataLength; i++)
	{
		if(data[i] > 127) _SerialObjOut->write((uint8_t)127);
		else _SerialObjOut->write((uint8_t)data[i]);
	}
	//Write End Command to MIDI Port:
	_SerialObjOut->write((uint8_t)SYSEX_END);
}
//Send MMC strings:

//Send SysEx strings:
//(SysEx messages can have as many lines as you want, and each line is a 7-bit (0-127) number.)
//Parameters: data (1+ length table [0-127]), dataLength (1+), boolean generateSysexStartStop Is 0xF/ alreafy in the data ?
void MidiPort::sendSysEx(byte data[], unsigned int dataLength, boolean generateSysexStartStop)
{
	//Write Command to MIDI Port:
	if (generateSysexStartStop) _SerialObjOut->write((uint8_t)SYSEX_START);
	//Write SysEx Data to MIDI Port:
	for(int i = 0; i < dataLength; i++)
	{
		if(data[i] > 127) _SerialObjOut->write((uint8_t)127);
		else _SerialObjOut->write((uint8_t)data[i]);
	}
	//Write End Command to MIDI Port:
	if (generateSysexStartStop)_SerialObjOut->write((uint8_t)SYSEX_END);
}
void MidiPort::sendSysEx(const char* data, boolean generateSysexStartStop)
{
	uint8_t dataLength = strlen(data);
	//Write Command to MIDI Port:
	if (generateSysexStartStop) _SerialObjOut->write((uint8_t)SYSEX_START);
	//Write SysEx Data to MIDI Port:
	for(int i = 0; i < dataLength; i++)
	{
		if(data[i] > 127) _SerialObjOut->write((uint8_t)127);
		else _SerialObjOut->write((uint8_t)data[i]);
	}
	//Write End Command to MIDI Port:
	if (generateSysexStartStop)_SerialObjOut->write((uint8_t)SYSEX_END);
}

//Send MMC strings:

void MidiPort::sendMMC_Command(uint8_t command)
{
	_SerialObjOut->write((uint8_t)SYSEX_START); 
	_SerialObjOut->write((uint8_t)0x7F);	
	_SerialObjOut->write((uint8_t)0x7F);
	_SerialObjOut->write((uint8_t)0x06);	
	_SerialObjOut->write((uint8_t)0x06);	// Bug in Serial, first 0x06 is getting lost
	_SerialObjOut->write((uint8_t)command);
	_SerialObjOut->write((uint8_t)SYSEX_END);
	_SerialObjOut->flush();
	_SerialObjOut->flush();
}





//--------------------------------------- Recieve MIDI Messages --------------------------------------

//----- Add Event Handlers -----

//Voice Messages:
void *MidiPort::handleMidiEvent(boolean (*handler)(const uint8_t, CommandType, Channel, byte[], uint8_t))
{

	void *fptr = &_handleMidiEvent;
	_handleMidiEvent = handler;
	return (void *) fptr;
}
void *MidiPort::handleNoteOn(void (*handler)(const uint8_t, Channel, byte, byte))
{
	void *fptr = &_handleNoteOn;
	_handleNoteOn = handler;
	return (void *) fptr;
}
void *MidiPort::handleNoteOff(void (*handler)(const uint8_t, Channel, byte, byte))
{
	void *fptr = &_handleNoteOff;
	_handleNoteOff = handler;
	return (void *) fptr;
}
void *MidiPort::handleKeyPressure(void (*handler)(const uint8_t, Channel, byte, byte))
{
	void *fptr = &_handleKeyPressure;
	_handleKeyPressure = handler;
	return (void *) fptr;
}
void *MidiPort::handleControlChange(void (*handler)(const uint8_t, Channel, byte, byte))
{
	void *fptr = &_handleControlChange;
	_handleControlChange = handler;
	return (void *) fptr;
}
void *MidiPort::handleProgramChange(void (*handler)(const uint8_t, Channel, byte))
{
	void *fptr = &_handleProgramChange;
	_handleProgramChange = handler;
	return (void *) fptr;
}
void *MidiPort::handleChannelPressure(void (*handler)(const uint8_t, Channel, byte))
{
	void *fptr = &_handleChannelPressure;
	_handleChannelPressure = handler;
	return (void *) fptr;
}
void *MidiPort::handlePitchBend(void (*handler)(const uint8_t, Channel, int16_t))
{
	void *fptr = &_handlePitchBend;
	_handlePitchBend = handler;
	return (void *) fptr;
}
//Mode Messages:
void *MidiPort::handleChannelMode(void (*handler)(const uint8_t, Channel, byte, byte))
{
	void *fptr = &_handleChannelMode;
	_handleChannelMode = handler;
	return (void *) fptr;
}
//System Messages:
void *MidiPort::handleSysEx(void (*handler)(const uint8_t, byte[], unsigned int, byte[], unsigned int))
{
	void *fptr = &_handleSysEx;
	_handleSysEx = handler;
	return (void *) fptr;
}
void *MidiPort::handleQuarterFrame(void (*handler)(const uint8_t, byte, byte))
{
	void *fptr = &_handleQuarterFrame;
	_handleQuarterFrame = handler;
	return (void *) fptr;
}
void *MidiPort::handleSongPosition(void (*handler)(const uint8_t, uint16_t))
{
	void *fptr = &_handleSongPosition;
	_handleSongPosition = handler;
	return (void *) fptr;
}
void *MidiPort::handleSongSelect(void (*handler)(const uint8_t, byte))
{
	void *fptr = &_handleSongSelect;
	_handleSongSelect = handler;
	return (void *) fptr;
}
void *MidiPort::handleTuneRequest(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleTuneRequest;
	_handleTuneRequest = handler;
	return (void *) fptr;
}
//Real-Time Messages:
void *MidiPort::handleTimingClock(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleTimingClock;
	_handleTimingClock = handler;
	return (void *) fptr;
}
void *MidiPort::handleStart(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleStart;
	_handleStart = handler;
	return (void *) fptr;
}
void *MidiPort::handleContinue(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleContinue;
	_handleContinue = handler;
	return (void *) fptr;
}
void *MidiPort::handleStop(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleStop;
	_handleStop = handler;
	return (void *) fptr;
}
void *MidiPort::handleActiveSense(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleActiveSense;
	_handleActiveSense = handler;
	return (void *) fptr;
}
void *MidiPort::handleReset(void (*handler)(const uint8_t ))
{
	void *fptr = &_handleReset;
	_handleReset = handler;
	return (void *) fptr;
}
#ifdef USE_NMIDI_INTERFACES
void *MidiPort::handleMidiEvent(IMidiEventHandler* p)
{
	void *fptr =pMidiEventHandler;
	pMidiEventHandler = p;
	return fptr;
};
void *MidiPort::handleNoteOn(INoteOnHandler* p)
{
	void *fptr =pNoteOnHandler;
	pNoteOnHandler = p;
	return fptr;
};
void *MidiPort::handleNoteOff(INoteOffHandler* p)
{
	void *fptr =pNoteOffHandler;
	pNoteOffHandler = p;
	return fptr;
};
void *MidiPort::handleKeyPressure(IKeyPressureHandler* p)
{
	void *fptr =pKeyPressureHandler;
	pKeyPressureHandler = p;
	return fptr;
};
void *MidiPort::handleControlChange(IControlChangeHandler* p)
{
	void *fptr =pControlChangeHandler;
	pControlChangeHandler = p;
	return fptr;
};
void *MidiPort::handleProgramChange(IProgramChangeHandler* p)
{
	void *fptr =pProgramChangeHandler;
	pProgramChangeHandler = p;
	return fptr;
};
void *MidiPort::handleChannelPressure(IChannelPressureHandler* p)
{
	void *fptr =pChannelPressureHandler;
	pChannelPressureHandler = p;
	return fptr;
};
void *MidiPort::handlePitchBend(IPitchBendHandler* p)
{
	void *fptr =pPitchBendHandler;
	pPitchBendHandler = p;
	return fptr;
};
//Mode Messages:
void *MidiPort::handleChannelMode(IChannelModeHandler* p)
{
	void *fptr =pChannelModeHandler;
	pChannelModeHandler = p;
	return fptr;
};
//System Messages:
void *MidiPort::handleSysEx(ISysExHandler* p)
{
	void *fptr =pSysExHandler;
	pSysExHandler = p;
	return fptr;
};
void *MidiPort::handleQuarterFrame(IQuarterFrameHandler* p)
{
	void *fptr =pQuarterFrameHandler;
	pQuarterFrameHandler = p;
	return fptr;
};
void *MidiPort::handleSongPosition(ISongPositionHandler* p)
{
	void *fptr =pSongPositionHandler;
	pSongPositionHandler = p;
	return fptr;
};
void *MidiPort::handleSongSelect(ISongSelectHandler* p)
{
	void *fptr =pSongSelectHandler;
	pSongSelectHandler = p;
	return fptr;
};
void *MidiPort::handleTuneRequest(ITuneRequestHandler* p)
{
	void *fptr =pTuneRequestHandler;
	pTuneRequestHandler = p;
	return fptr;
};
//Real-Time Messages:
void *MidiPort::handleTimingClock(ITimingClockHandler* p)
{
	void *fptr =pTimingClockHandler;
	pTimingClockHandler = p;
	return fptr;
};
void *MidiPort::handleStart(IStartHandler* p)
{
	void *fptr =pStartHandler;
	pStartHandler = p;
	return fptr;
};
void *MidiPort::handleContinue(IContinueHandler* p)
{
	void *fptr =pContinueHandler;
	pContinueHandler = p;
	return fptr;
};
void *MidiPort::handleStop(IStopHandler* p)
{
	void *fptr =pStopHandler;
	pStopHandler = p;
	return fptr;
};
void *MidiPort::handleActiveSense(IActiveSenseHandler* p)
{
	void *fptr =pActiveSenseHandler;
	pActiveSenseHandler = p;
	return fptr;
};
void *MidiPort::handleReset(IResetHandler* p)
{
	void *fptr =pResetHandler;
	pResetHandler = p;
	return fptr;
};

#endif // USE_NMIDI_INTERFACES
//------------------------------------------ Extra Functions -----------------------------------------

//Get the expected length, in bytes, of a particular type of event when completed:
//(Returns -1 if cmd is SysEx or INVALID)
int8_t MidiPort::getEventDataLength(CommandType cmd)
{
	int8_t paramCount = -1;
	switch(cmd)
	{
	case NOTE_OFF:
	case NOTE_ON:
	case POLY_AT:
	case CTRL_CHANGE:
	case CHANNEL_MODE:

	case CHANNEL_AT:
	case PITCH_BEND:
		paramCount = 2;
		break;
	case PROG_CHANGE:
		paramCount = 1;
		break;

	//System Messages:
	case SYSEX_START:
	case SYSEX_END:
		paramCount = -2;
		break;

	case Q_TIME_FRAME:
		paramCount = 2;
		break;
	case SONG_POS:
		paramCount = 2;
		break;
	case SONG_SELECT:
		paramCount = 2;
		break;
	case TUNE_REQ:
		paramCount = 0;
		break;

	//Real-Time Messages:

	case TIMING_CLOCK:
	case ACTIVE_SENSE:
	case START:
	case CONTINUE:
	case STOP:
	case INVALID:
	case SOFT_RESET:
		paramCount = 0;
		break;
	}
	return paramCount;
}

//Convert CommandType to string:
String MidiPort::commandTypeToString(const CommandType cmd)
{
	String cmdName;
	switch(cmd)
	{
	//Voice & Mode Messages:
	case NOTE_OFF:
		cmdName = F("NOTE_OFF");
		break;
	case NOTE_ON:
		cmdName = F("NOTE_ON ");
		break;
	case POLY_AT:
		cmdName = F("POLY_AT ");
		break;
	case CTRL_CHANGE:
		cmdName = F("CTRL_CHANGE");
		break;
	case CHANNEL_MODE:
		cmdName = F("CHANNEL_MODE");
		break;
	case PROG_CHANGE:
		cmdName = F("PROG_CHANGE");
		break;
	case CHANNEL_AT:
		cmdName = F("CHANNEL_AT");
		break;
	case PITCH_BEND:
		cmdName = F("PITCH_BEND");
		break;
	//System Messages:
	case SYSEX_START:
		cmdName = F("SYSEX_START");
		break;
	case Q_TIME_FRAME:
		cmdName = F("Q_TIME_FRAME");
		break;
	case SONG_POS:
		cmdName = F("SONG_POS");
		break;
	case SONG_SELECT:
		cmdName = F("SONG_SELECT");
		break;
	case TUNE_REQ:
		cmdName = F("TUNE_REQ");
		break;
	case SYSEX_END:
		cmdName = F("SYSEX_END");
		break; //<-- FOR DEBUG
	//Real-Time Messages:
	case TIMING_CLOCK:
		cmdName = F("TIMING_CLOCK");
		break;
	case START:
		cmdName = F("START");
		break;
	case CONTINUE:
		cmdName = F("CONTINUE");
		break;
	case STOP:
		cmdName = F("STOP");
		break;
	case ACTIVE_SENSE:
		cmdName = F("ACTIVE_SENSE");
		break;
	case SOFT_RESET:
		cmdName = F("SOFT_RESET");
		break;
	//Other:
	case NO_NEW_MSG:
		cmdName = F("NO_NEW_MSG");
		break; //<-- FOR DEBUG
	default:
		cmdName = F("INVALID");
	}
	return cmdName;
}

//----------------------------------------- Private Functions ----------------------------------------


//Central function to check for and process MIDI data:


//Get CommandType from cmd number.
CommandType MidiPort::getEventCmdType(byte status)
{
	if ((status > 0x7f) && (status < 0xf0))
	{
		byte cmd2 = status & 0xf0;
		return (CommandType)cmd2;
	}
	if ((status >= 0xf0))
	{
		return (CommandType)status;
	}
	return INVALID;
}

//Determine how to forward traffic.
void MidiPort::forwardTraffic(byte newByte)
{
	if(_thruMode == FORWARD_OFF) return;   //FORWARD_OTHER:

	if(_thruMode == FORWARD_OTHER)   //FORWARD_OTHER:
	{
		if(_listenCh == CH_ALL)   //If listen to all channels:
		{
			if(_activeChannel == CH_ALL)   //If no channel:
			{
				_SerialObjOut->write((uint8_t)newByte);
			}
		}
		else     //If listen to one channel:
		{
			if(_activeChannel != _listenCh || _activeChannel == CH_ALL)   //If on different channel or no channel:
			{
				_SerialObjOut->write((uint8_t)newByte);
			}
		}
	}
	else if(_thruMode == FORWARD_SELF)     //FORWARD_SELF:
	{
		if(_listenCh == CH_ALL)   //If listen to all channels:
		{
			if(_activeChannel != CH_ALL)   //If has channel:
			{
				_SerialObjOut->write((uint8_t)newByte);
			}
		}
		else     //If listen to one channel:
		{
			if(_activeChannel == _listenCh || _activeChannel == CH_ALL)   //If on same channel or no channel:
			{
				_SerialObjOut->write((uint8_t)newByte);
			}
		}
	}
}

//Check if handlers should be allowed to run.
boolean MidiPort::flt_channel()
{
	if(_listenCh == CH_ALL) return true; //<-- If listen to all channels. vvv If listen to one channel.
	else if(_activeChannel == _listenCh || _activeChannel == CH_ALL) return true; //If on same channel or no channel.
	else return false;
}
boolean MidiPort::processSysex(byte newByte )
{
	return false;
}
boolean MidiPort::processRPN(byte newByte )
{
	return false;
}
boolean MidiPort::processNRPN(byte newByte )
{
	return false;
}
inline void MidiPort::writeMsg(uint8_t msg[], uint8_t len)
{

	//Serial.print("writeMsg len ");
	//Serial.println(len );
	if (_runningStatus)
	{
		if (len == 3)
		{
			if (msg[0] == _msg_lastSent)
			{
				// first attempt to fix enable running status....
				_SerialObjOut->write(&(msg[1]),2);
				//Serial.println("W1");
			}
			else
			{
				_SerialObjOut->write(msg, len);
				//Serial.println("W2");
			}
			_msg_lastSent = msg[0];
		}
		else
		{
			_msg_lastSent = 0;
		}
	}
	else
	{
		_msg_lastSent = 0;
		_SerialObjOut->write(msg, len);
		//Serial.println("W3");
	}
#ifdef IMMEDIATE_SEND
	_SerialObjOu->flush();
#endif
}
void MidiPort::writeMsg(uint8_t msg)
{
	_msg_lastSent = 0;
	_SerialObjOut->write(msg);
#ifdef IMMEDIATE_SEND
	_SerialObjOut->flush();
#endif
}
void MidiPort::writeCommand(CommandType msg)
{
	_msg_lastSent = 0;
	_SerialObjOut->write(msg);
#ifdef IMMEDIATE_SEND
	_SerialObjOut->flush();
#endif
}
