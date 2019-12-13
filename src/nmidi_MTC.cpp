/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#include <Stream.h>
#include <Arduino.h>

#include <NMidi.h>
#include <NMidi_mtc.h>

#ifdef USE_TIMER_THREE_MIDI_CLOCK
#include <TimerThree.h>
#define TIMER Timer3

#else
#include <TimerOne.h>
#define TIMER Timer1

#endif


using namespace nmidi;

MTCSession::MTCSession(  MidiPort *_port)
{
    _midiPort = _port;
};
void MTCSession::start()
{}
void MTCSession::stop()
{}
void MTCSession::cont()
{}
void MTCSession::reset()
{
	_hh = _mm = _ss = _ff = _frames = 0;
}
void MTCSession::fps(MTCFrames fps)
{
		_fps = fps;
}  // 24,25,29,30
void MTCSession::locate(int8_t hh, int8_t mm, int8_t ss, int8_t ff)
{
	_hh = hh;
	_mm = mm;
	_ss = ss;
	_ff = ff;
	}
long MTCSession::microsPerTick(MTCFrames fps)
{
	_fps = fps;
	//25 -> 100
	//24 ->
	//30 ->
	//29
	return _mpt;
	
	
}
long MTCSession::microsPerTick()
{
	
	//25 -> 100
	//24 ->
	//30 ->
	//29
	return _mpt;
		
}


MTCFrames MTCSession::fps()
{
	return _fps;
}
/** sendMTCFullFrameMessage
 * 
 * decorated port.sendMTCTimeFrame
 */
void MTCSession::sendMTCQuarterMessage()
{
	_ctr = _midiPort->sendMTCTimeFrame(_ctr, _hh,_mm,_ss,_ff,_fps);
}
/** sendMTCFullFrameMessage
 * 
 * The full frame message carries complete timing information in the MIDI time code format 
 * (i.e., hours:minutes:seconds:frames) and thus it is similar to the MIDI quarter frame message. 
 * The difference between the full frame and quarter frame message is that 
 * while the quarter frame message carries a piece of the MIDI time code 
 * the full frame message carries the complete MIDI time code. 
 * Quarter frame messages are smaller and are used during playback to synchronize devices. 
 * Full frame messages are too big and are not used during playback,
 * as during playback they may clog up the MIDI connection.
 * 
 * @param hh [0 .. 127]
 *   Hour
 * @param mm [0 .. 59]
 *   mm
 * @param ss [0 .. 59]
 *     Seconds
 * @param ff [0 .. fps]
 *     Frames
 */ 
void MTCSession::sendMTCFullFrameMessage()
{
	
  //F0 7F 7F 01 01 hh mm ss ff F7
  byte buf[10] =
  {
    0xF0,
    0x7F,
    0x7F,
    0x01,
    0x01,
    (byte)(0x20 | _hh ),
    _mm,
    _ss,
    _ff,
    0xF7
  };
   _midiPort->writeMsg(buf, 10);
}
