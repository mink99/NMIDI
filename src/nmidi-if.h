#ifndef NMIFI_IF_H
#define NMIFI_IF_H

namespace nmidi
{
		class IMidiEventHandler
		{
			public:
				virtual boolean _handleMidiEvent(const uint8_t portID, CommandType cmd, Channel ch, byte *data, uint8_t dataLength) = 0;  //command, Channel ch, rawEventData (0-127 [1+]), dataLength (1+)
		};
        class INoteOnHandler
		{
			public:
				virtual void _handleNoteOn(const uint8_t portID, const Channel ch, const byte key, const byte velocity) = 0; //Channel ch, key (0-127), velocity (0-127)
		};
        class INoteOffHandler
		{
			public:
				virtual void _handleNoteOff(const uint8_t portID, const Channel ch, const byte key, const byte velocity) = 0; //Channel ch, key (0-127), velocity (0-127)
		};
        class IKeyPressureHandler
		{
			public:
				virtual void _handleKeyPressure(const uint8_t portID, const Channel ch, const byte key, const byte afterTouchPressure) = 0; //Channel ch, key (0-127), afterTouchPressure (0-127)
		};
        class IControlChangeHandler
		{
			public:
				virtual void _handleControlChange(const uint8_t portID, const Channel ch, const byte controller, const byte value) = 0; //Channel ch, controller (0-119), value (0-127)
		};
        class IProgramChangeHandler
		{
			public:
				virtual void _handleProgramChange(const uint8_t portID, const Channel ch, const byte program) = 0; //Channel ch, program (0-127)
		};
        class IChannelPressureHandler
		{
			public:
				virtual void _handleChannelPressure(const uint8_t portID, const Channel ch, const byte afterTouchPressure) = 0; //Channel ch, afterTouchPressure (0-127)
		};
        class IPitchBendHandler
		{
			public:
				virtual void _handlePitchBend(const uint8_t portID, const Channel ch, const int16_t wheelValue) = 0; //Channel ch, wheelValue (-8192-8191)
		};
        //Mode Messages:
        class IChannelModeHandler
		{
			public:
				virtual void _handleChannelMode(const uint8_t portID, const Channel ch, const byte settingCode, const byte value) = 0; //Channel ch, settingCode (120-127), value (0-127)
		};
        //System Messages:
        class ISysExHandler
		{
			public:
				virtual void _handleSysEx(const uint8_t portID, const byte* idCode, const uint16_t idCodeLength, const byte* data, const uint16_t dataLength) = 0; //idCode[] (0-127 [1-3]), idCodeLength (1-3), data[] (0-127 [1+]), dataLength (1+)
		};
        class IQuarterFrameHandler
		{
			public:
				virtual void _handleQuarterFrame(const uint8_t portID, const byte msgType, const byte values) = 0; //msgType (0-7), values (0-15)
		};
        class ISongPositionHandler
		{
			public:
				virtual void _handleSongPosition(const uint8_t portID, const uint16_t position) = 0; //position (0-16383)
		};
        class ISongSelectHandler
		{
			public:
				virtual void _handleSongSelect(const uint8_t portID, const byte song) = 0; //song (0-127)
		};
        class ITuneRequestHandler
		{
			public:
				virtual void _handleTuneRequest(const uint8_t portID) = 0;  //[No Parameters]
		};
        //Real-Time Messages:
        class ITimingClockHandler
		{
			public:
				virtual void _handleTimingClock(const uint8_t portID) = 0;  //[No Parameters]
		};
        class IStartHandler
		{
			public:
				virtual void _handleStart(const uint8_t portID) = 0;  //[No Parameters]
		};
        class IContinueHandler
		{
			public:
				virtual void _handleContinue(const uint8_t portID) = 0;  //[No Parameters]
		};
        class IStopHandler
		{
			public:
				virtual void _handleStop(const uint8_t portID) = 0;  //[No Parameters]
		};
        class IActiveSenseHandler
		{
			public:
				virtual void _handleActiveSense(const uint8_t portID) = 0;  //[No Parameters]
		};
        class IResetHandler
		{
			public:
				virtual void _handleReset(const uint8_t portID) = 0;  //[No Parameters]
		};


	
}
#endif