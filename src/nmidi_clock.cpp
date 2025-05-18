/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */




#include <Stream.h>
#include <Arduino.h>

#include <NMidi.h>
#include <NMidi_clock.h>


#ifdef __SAM3X8E__
#pragma message "__SAM3X8E__ DUE"
	#include <DueTimer.h>
	#define TIMER Timer1
	#define initialize(n) setPeriod(n)
#endif
#ifdef __AVR__
#pragma message "__AVR__ UNO or NANO or MEGA"
	#ifdef USE_TIMER_THREE_MIDI_CLOCK
		#include <TimerThree.h>
		#define TIMER Timer3
	#else	
		#include <TimerOne.h>
		#define TIMER Timer1
	#endif
#endif
#ifdef ESP8266
	#pragma message "__AVR__ NODEMCU"
	#include <ESP8266_HW_ISRTimer.h>
	#define TIMER _ESP8266_HW_ISRTimer 
#endif	




using namespace nmidi;



MidiClockSession::MidiClockSession(  MidiPort *_port)
{
    _midiPort = _port;
    TIMER.initialize(bpmToTick());
    TIMER.stop();
};
void MidiClockSession::continuous(boolean b)
{
    _continuous = b;
};
void MidiClockSession::start()
{
    if (_mode == mode_stop)
    {
        _mode = mode_armed;
    }
    else
    {
        return;
    };
    if (_continuous)
    {
        
        _mode = mode_run;
    }
    else
    {
		_counter = 92;		
        _mode = mode_run;
    }
};
void MidiClockSession::stop()
{
	if (_mode == mode_run) 
	{
		_mode = mode_stop;
		_midiPort->sendStop();
	}
};
void MidiClockSession::cont()
{

};
void MidiClockSession::reset()
{
    _counter = _beats = 0;
};
long MidiClockSession::ticks()
{
    return _counter;
};
void MidiClockSession::bpm(uint8_t bpm)
{
    _bpm = bpm;
    TIMER.stop();
    TIMER.setPeriod(bpmToTick());
    TIMER.start();
};
void MidiClockSession::begin(uint8_t bpm, boolean continuous)
{
    _bpm = bpm;
    _continuous = continuous;
    
    TIMER.setPeriod(bpmToTick());
    TIMER.start();
};
void MidiClockSession::locate(int16_t beats)
{
    _beats = beats;
    _counter = 0;
};
long MidiClockSession::beats()
{
    return _beats;
};
long MidiClockSession::bpmToTick()
{
    _cpt = magic_number / (long)_bpm;
    return _cpt;
}
long MidiClockSession::bpmToTick(uint8_t bpm)
{
    _bpm = bpm;
    return magic_number / (long)_bpm;
}

void MidiClockSession::isr()
{
    _counter++;
    if (_mode == mode_armed)
    {
		if (_counter == 94) 
		{
			_midiPort->sendSPP(_beats);
		}
        if (_counter == 95) 
        {
			_midiPort->sendStart();
		}
	}
    if (_counter > 95) _counter = 0;
    if (_mode == mode_run) 
    {
		if (!(_counter / 4)) _beats++;
	}
    if (_continuous)
    {
        _midiPort->timingClock();
    } 
    else
    {
        if (_mode == mode_run) _midiPort->timingClock();
    }
};
uint8_t MidiClockSession::bpm()
{
    return _bpm;
};
boolean MidiClockSession::isShuffleTrigger (int8_t _tick, int8_t _shift, int8_t _res )
    {
      switch (_res)
      {
        case 8:
          if (_tick % 24 == 0) return true;   //quarters are no shifted;
          if (_tick % 12 == _shift) return true;   // 1/8 are shifted;
          break;
        case 12:
          if (_tick % 24 == 0) return true;       //quarters are no shifted;
          if (_tick % 8 == _shift) return true;   // 1/8T are  shifted;
          break;
        case 16:
          if (_tick % 24 == 0) return true;       //quarters are no shifted;
          if (_tick % 6 == _shift) return true;   // 1/16 are  shifted;
          break;
      }

      return false;
    }
