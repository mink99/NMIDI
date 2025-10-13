#ifndef NMIDI_CLOCKPROVIDER_H
#define NMIDI_CLOCKPROVIDER_H

#include <NMidi.h>



namespace nmidi
{

const long magic_number_4_4 = 2500000;
const long magic_number_3_4 = 1875000;

long bpm2Ticks(uint8_t __bpm, uint8_t resolution = 4)
{
	if (resolution == 3) return magic_number_3_4 / (long)__bpm;
	return magic_number_4_4 / (long)__bpm;

}
long bpm2MTC(uint8_t __bpm, uint8_t resolution = 4, uint8_t fps = 30)
{
	long ticks;
	if (resolution == 3)
	{
		ticks = magic_number_3_4 / (long)__bpm;
	}
	else
	{
		ticks = magic_number_4_4 / (long)__bpm;
	};
	// ticks are millisecond -> so ticks * fps ....
	ticks = ticks *1000;
	ticks = ticks / fps;
	return 0;

}




//class nmidi::MidiTimerBase;
//class nmidi::MidiPort;
void isrFunc();
void eventFunc(byte b);

class MidiTimerBase;
MidiTimerBase* pClockprovider = nullptr;
MidiPort* pClockPort = nullptr;

class MidiTimerBase
{
public:
	MidiTimerBase()
	{
		//if (pClockprovider) cerr << "only one instance of Timerbase";
		pClockprovider = this;		
		Serial.print("PORT ");
		Serial.println((long)pClockPort,HEX);
	}
	MidiTimerBase(MidiPort *pPortInstance)
	{
		//if (pClockprovider) cerr << "only one instance of Timerbase";
		pClockprovider = this;
		pClockPort = pPortInstance;
		port = pPortInstance;		
		Serial.print("PORT ");
		Serial.println((long)pClockPort,HEX);
	}
	void assignPort(MidiPort *pPortInstance)
	{
		port = pPortInstance;
		pClockPort = pPortInstance;
	}
	void unAssignPort()
	{
		port = nullptr;
		pClockPort = nullptr;
	}
	virtual void init() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void reset() = 0;


	void setBPM(uint8_t __bpm)
	{
		_bpm = __bpm;
		stop();
		setPeriod(bpm2Ticks(_bpm));
		start();
	}
	uint8_t getBPM()
	{
		return _bpm;
	}

	long ticks()
	{
		return elapsed_ticks;
	}
	MidiPort *getPort()
	{
		return port;
	}


protected:

	virtual void attachInterrupt(void (*isr)()) = 0;
	MidiPort *port = nullptr;
	int8_t _bpm = 120;
	volatile long elapsed_ticks = 0;
	volatile int8_t fps = 30;


	long bpmToTicks()
	{
		return bpm2Ticks(_bpm);
	};

	virtual void setPeriod(long ticks) = 0;
	void setTicks(uint8_t _ticks = 0)
	{
		elapsed_ticks = _ticks;
	}
private:
	int8_t portCount = 0;
};

};//Namespace
#if defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4)
#pragma message "UNO MINIMA WIFI FSPTIMER CBTimer.h"
#include <FspTimer.h>
#define getHardwareClock(port) Midi_UNOR4Impl CLK(port)

#include ".\hardware\clock_minima.hpp"
#endif

#ifdef __SAM3X8E__
#pragma message "__SAM3X8E__ DUE use DueTimer"
#include <DueTimer.h>
#define getHardwareClock(port) Midi_DUEImpl CLK(port)
#include ".\hardware\clock_SAM3X8E.hpp"
#endif

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MEGA2560)
#pragma message "__AVR__ UNO or NANO or MEGA : Use TIMER1"
#include <TimerOne.h>
#define getHardwareClock(port) Midi_AVRImpl CLK(port)
#include "./hardware/clock_AVR.hpp"
#endif

#ifdef ESP32
#pragma message "ESP32: Use TIMER1"
#include <TimerOne.h>
#define getHardwareClock(port) Midi_ESP32Impl CLK(port)
#include "./hardware/clock_esp32.hpp"
#endif

#ifdef ESP8266
#pragma message "ESP8266: use HW_ISRTimer"

#include <ESP8266TimerInterrupt.h>
#define getHardwareClock(port) Midi_ESP8266Impl CLK(port)
#include "./hardware/clock_esp8266.hpp"
#endif

namespace nmidi
{

void isrFunc()
{
		if (pClockPort)
		{				
			pClockPort->internalClockEvent(0XF0);
		}
	
};
void eventFunc(byte b)
{
		if (pClockPort)
		{
			pClockPort->internalClockEvent(b);
		}
	
};//Namespace
}






#endif //Guard