/** MidiPort Libary

based on ArduMidi C++ FILE v2.1 by Pecacheu
NMidi Libary C++ FILE v2.1 by Mink

 */

#ifndef NMIDI_h
#define NMIDI_h

#include <stream.h>
#include <Arduino.h>




#ifndef midi_utils_h
#define midi_utils_h

// Some useful stuff
#define MSB(a)  0x7f & (a >> 7)
#define LSB(a)  0x7f & a

#define msg_status _msg[0]
#define msg_param1 _msg[1]
#define msg_param2 _msg[2]


#define LO_NIBBLE(b) (byte)(b & 0x0F)
#define HI_NIBBLE(b) (byte)((b & 0xF0) >> 4 )


unsigned short combineBytes(unsigned char _msb, unsigned char _lsb);
const char *decodeNote (byte note);
byte decodeOct (byte note);
char *decodeNote2 (byte note);

#endif
namespace nmidi
{



#define CC_Bank_Select	0			// Allows user to switch bank for patch selection. Program change used with Bank Select. MIDI can access 16,384 patches per MIDI channel.
#define CC_Modulation	1			//Generally this CC controls a vibrato effect (pitch, loudness, brightness). What is modulated is based on the patch.
#define CC_Breath_Controller	2	//Often times associated with aftertouch messages. It was originally intended for use with a breath MIDI controller in which blowing harder produced higher MIDI control values. It can be used for modulation as well.
#define CC_Foot_Controller	4		//Often used with aftertouch messages. It can send a continuous stream of values based on how the pedal is used.
#define CC_Portamento_Time	5		//Controls portamento rate to slide between 2 notes played subsequently.
#define CC_Data_Entry_MSB	6		//Controls Value for NRPN or RPN parameters.
#define CC_Volume	7				//Control the volume of the channel
#define CC_Balance	8				//Controls the left and right balance, generally for stereo patches. 0 = hard left, 64 = center, 127 = hard right
#define CC_Pan	10					// Controls the left and right balance, generally for mono patches.0 = hard left, 64 = center, 127 = hard right
#define CC_Expression	11			//Expression is a percentage of volume (CC7).
#define CC_Effect_Controller_1	12	//Usually used to control a parameter of an effect within the synth/workstation.
#define CC_Effect_Controller_2	13	//Usually used to control a parameter of an effect within the synth/workstation.
#define CC_Damper_Pedal 64			// 0 to 63 = Off, 64 to 127 = On
#define CC_Portamento 65			//OnOff switch 0 to 63 = Off, 64 to 127 = On
#define CC_Sostenuto  66			// On/Off Switch	On/Off switch – Like the Sustain controller (CC 64), However it only holds notes that were “On” when the pedal was pressed. People use it to “hold” chords” and play melodies over the held chord.
#define CC_Soft_Pedal 67			// On/Off switch - Lowers the volume of notes played. 0 to 63 = Off, 64 to 127 = On
#define CC_Legato_FootSwitch	68	// On/Off switch - Turns Legato effect between 2 subsequent notes On or Off. 0 to 63 = Off, 64 to 127 = On
#define CC_Hold_2	69				// Another way to “hold notes” (see MIDI CC 64 and MIDI CC 66). However notes fade out according to their release parameter rather than when the pedal is released.
#define CC_Sound_Controller_1	70	//Usually controls the way a sound is produced. Default = Sound Variation.
#define CC_Sound_Controller_2	71	//Allows shaping the Voltage Controlled Filter (VCF). Default = Resonance -also(Timbre or Harmonics)
#define CC_Sound_Controller_3	72	//Controls release time of the Voltage controlled Amplifier (VCA). Default = Release Time.
#define CC_Sound_Controller_4	73	//Controls the “Attack’ of a sound. The attack is the amount of time it takes forthe sound to reach maximum amplitude.
#define CC_Sound_Controller_5	74	//Controls VCFs cutoff frequency of the filter.
#define CC_Sound_Controller_6	75	//Generic – Some manufacturers may use to further shave their sounds.
#define CC_Sound_Controller_7	76	//Generic – Some manufacturers may use to further shave their sounds.
#define CC_Sound_Controller_8	77	//Generic – Some manufacturers may use to further shave their sounds.
#define CC_Sound_Controller_9	78	//Generic – Some manufacturers may use to further shave their sounds.
#define CC_Sound_Controller_10	79	//Generic – Some manufacturers may use to further shave their sounds.
#define CC_General_Purpose_1  80	//MIDI CC Controller	Generic On/Off switch 0 to 63 = Off, 64 to 127 = On
#define CC_General_Purpose_2  81	//MIDI CC Controller	Generic On/Off switch 0 to 63 = Off, 64 to 127 = On
#define CC_General_Purpose_3  82	//MIDI CC Controller	Generic On/Off switch 0 to 63 = Off, 64 to 127 = On
#define CC_General_Purpose_4  83	//MIDI CC Controller	Generic On/Off switch 0 to 63 = Off, 64 to 127 = On
#define CC_Portamento_level 84			// CC Control	Controls the amount of Portamento.
#define CC_Effect_1_Depth	91		//Usually controls reverb send amount
#define CC_Effect_2_Depth	92		//Usually controls tremolo amount
#define CC_Effect_3_Depth	93		//Usually controls chorus amount
#define CC_Effect_4_Depth	94		//Usually controls detune amount
#define CC_Effect_5_Depth	95		//Usually controls phaser amount
#define CC_Data_Increment	96		//Usually used to increment data for RPN and NRPN messages.
#define CC_Data_Decrement	97		//Usually used to decrement data for RPN and NRPN messages.

    //Receivable Message Types:
    enum CommandType
    {
        //internal Messages:
        NO_NEW_MSG       	= 0x00, ///< No New Events Recieved
        INVALID          	= 0x01, ///< Invalid or Badly Formatted Event Recieved!
        CHANNEL_MODE     	= 0x02, ///< Channel Mode (Uses Same Command ID as CTRL_CHANGE)
        DATABYTE            = 0x03, ///< DATA recieved
        //Voice & Mode Messages:
        NOTE_OFF         	= 0x80, ///< NoteOff Event
        NOTE_ON          	= 0x90, ///< NoteOn Event
        POLY_AT     		= 0xA0, ///< Key AfterTouch
        CTRL_CHANGE      	= 0xB0, ///< Control Change
        PROG_CHANGE     	= 0xC0, ///< Program Change
        CHANNEL_AT 		= 0xD0, ///< Channel AfterTouch
        PITCH_BEND       	= 0xE0, ///< Pitch Bend Wheel Change
        //System Messages:
        SYSEX_START      = 0xF0, ///< Begin SysEx Dump
        Q_TIME_FRAME     = 0xF1, ///< Qarter Time Frame
        SONG_POS         = 0xF2, ///< SPP
        SONG_SELECT      = 0xF3, ///< Does something or the other...
        TUNE_REQ         = 0xF6, ///< Tune Your Insturments!
        SYSEX_END        = 0xF7, ///< End SysEx Dump
        //Real-Time Messages:
        TIMING_CLOCK     = 0xF8, ///< Sent 24 Times per Quarter Note When SYNC is Enabled
        START            = 0xFA, ///< Start
        CONTINUE         = 0xFB, ///< Continue
        STOP             = 0xFC, ///< Stop
        ACTIVE_SENSE     = 0xFE, ///< Just don't use it. It's pointless.
        SOFT_RESET       = 0xFF, ///< Perform Soft Reset
    };
    enum NRPNType
    {
        NRPN_LSB		= 0x62,
        NRPN_MSB		= 0x63,
        RPN_LSB		    = 0x64,
        RPN_MSB			= 0x65,
        NRPN_TERM		= 0xFF,
    };
    //Possible Channels:
    enum Channel {CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16, CH_NONE, CH_ALL};
    //Traffic Forwarding Modes:
    enum ThruMode
    {
        FORWARD_OFF,   ///< Thru Disabled. Nothing received on the MIDI IN port is forwarded to the MIDI OUT port.
        FORWARD_OTHER, ///< Forwards received commands to MIDI OUT unless they're on the listening channel. This makes the most sense, and is the default setting.
        FORWARD_SELF,  ///< Forwards received commands to MIDI OUT only if they are on the listening channel.
        FORWARD_ALL,   ///< Any traffic received on MIDI IN is immediately forwarded to MIDI OUT. Warning, may cause a feedback loop in some setups!
    };

    enum MTCFrames
    {
        MTC_24FPS = 24,   ///< Thru Disabled. Nothing received on the MIDI IN port is forwarded to the MIDI OUT port.
        MTC_25FPS = 25, ///< Forwards received commands to MIDI OUT unless they're on the listening channel. This makes the most sense, and is the default setting.
        MTC_29FPS = 29,  ///< Forwards received commands to MIDI OUT only if they are on the listening channel.
        MTC_30FPS = 30,   ///< Any traffic received on MIDI IN is immediately forwarded to MIDI OUT. Warning, may cause a feedback loop in some setups!
    };
    /** @defgroup Main MidiPort
     *  These Classes are Used For core features
     *  @{
     */

    class MidiPort
    {
    public:
        //---- Setup ----
        MidiPort(Stream &serialObject, Channel listenCh = CH_ALL);
        int8_t getPortID()
        {
            return _portID;
        };
        void begin(int8_t id = -1) ;
        void enableRunningStatus(boolean enable = true) ///< use running status on send
        {
            _runningStatus = enable;
        };
        void setInputChannelFilter(Channel);
        void setThruMode(ThruMode);
        //---- Extra ----
        String commandTypeToString(CommandType);
        int8_t keysPressed()  ///< number of keys pressed at the moment
        {
            return _keysPressed;
        };

        //Voice Messages:
        void sendNoteOn(Channel, byte, byte);
        void sendKeyPressure(Channel, byte, byte);
        void sendNoteOff(Channel, byte, byte velocity = 0);
        void sendControlChange(Channel, byte, byte);
        void sendControlChangeHi(Channel, byte, int16_t);
        void sendProgramChange(Channel, byte);
        void sendChannelPressure(Channel, byte);
        void sendPitchBend(Channel, int16_t);
        void sendPitchBendCenter(Channel);
        //Mode Messages:
        void modeChange(Channel, byte, byte);
        //System Messages:
        void sendSysEx(byte[], unsigned int, byte[], unsigned int);
        void sendQuarterTimeFrame(byte, byte);
        void sendSPP(uint16_t);
        void sendSongSelect(byte);
        void sendTuneRequest();
        //Real-Time Messages:
        void timingClock();
        void sendStart();
        void sendContinue();
        void sendStop();
        void activeSense();

        // utilities sends
        void sendBankChange(Channel, byte , byte );
        uint8_t sendMTCTimeFrame(uint8_t counter , byte hh, byte mm, byte ss, byte ff, MTCFrames fps );
        //  “Channel Mode Messages.
        void sendAllSoundsOff(Channel );	//0x78
        void sendResetAll( Channel );	//0x79
        void sendLocalControl(Channel , boolean);	//0x7a
        void sendAllNotesOff(Channel );	//0x7b
        void sendOmniOff(Channel); //0x7c
        void sendOmniOn(Channel); //0x7d
        void sendMonoOn(Channel ch, uint8_t voices = 0); //0x7e
        void sendPolyOn(Channel); //0x7f
        // blockwise send
        void beginTransferSysex(byte, byte ); //0xf0
        void transferSysex(byte[], uint8_t); //0xf7
        uint8_t transferSysexR(byte[], uint8_t, uint8_t); //0xf7
        void endTransferSysex(); //0xf7
        void endTransferSysexR(byte ); //0xf7

        // status	99	NRPN MSB	98	NRPN LSB	06	value	101	127	100	12
        void beginTransferNRPN(Channel, byte, byte); //0x65
        void transferNRPN(Channel,  byte, byte); //0xf7
        void endTransferNRPN(Channel); //0x63 0xf7, 0xf7

        //---- Recieve MIDI Messages ----
        //--- Trigger Callbacks:
        void scanForData();
        //--- Set Callbacks:
        //Voice Messages:
        void *handleMidiEvent(boolean (*handler)(const uint8_t, CommandType, Channel, byte[], uint8_t));  //command, channel, rawEventData (0-127 [1+]), dataLength (1+)
        void *handleNoteOn(void (*handler)(const uint8_t , Channel, byte, byte)); //channel, key (0-127), velocity (0-127)
        void *handleNoteOff(void (*handler)(const uint8_t , Channel, byte, byte)); //channel, key (0-127), velocity (0-127)
        void *handleKeyPressure(void (*handler)(const uint8_t , Channel, byte, byte)); //channel, key (0-127), afterTouchPressure (0-127)
        void *handleControlChange(void (*handler)(const uint8_t , Channel, byte, byte)); //channel, controller (0-119), value (0-127)
        void *handleProgramChange(void (*handler)(const uint8_t , Channel, byte)); //channel, program (0-127)
        void *handleChannelPressure(void (*handler)(const uint8_t , Channel, byte)); //channel, afterTouchPressure (0-127)
        void *handlePitchBend(void (*handler)(const uint8_t , Channel, int16_t)); //channel, wheelValue (-8192-8191)
        //Mode Messages:
        void *handleChannelMode(void (*handler)(const uint8_t , Channel, byte, byte)); //channel, settingCode (120-127), value (0-127)
        //System Messages:
        void *handleSysEx(void (*handler)(const uint8_t , byte[], unsigned int, byte[], unsigned int)); //idCode[] (0-127 [1-3]), idCodeLength (1-3), data[] (0-127 [1+]), dataLength (1+)
        void *handleQuarterFrame(void (*handler)(const uint8_t , byte, byte)); //msgType (0-7), values (0-15)
        void *handleSongPosition(void (*handler)(const uint8_t , uint16_t)); //position (0-16383)
        void *handleSongSelect(void (*handler)(const uint8_t , byte)); //song (0-127)
        void *handleTuneRequest(void (*handler)(const uint8_t));  //[No Parameters]
        //Real-Time Messages:
        void *handleTimingClock(void (*handler)(const uint8_t ));  //[No Parameters]
        void *handleStart(void (*handler)(const uint8_t ));  //[No Parameters]
        void *handleContinue(void (*handler)(const uint8_t ));  //[No Parameters]
        void *handleStop(void (*handler)(const uint8_t ));  //[No Parameters]
        void *handleActiveSense(void (*handler)(const uint8_t ));  //[No Parameters]
        void *handleReset(void (*handler)(const uint8_t ));  //[No Parameters]

 // unoptimised write
        // encapsulate Serial object for USB
		void writeMsg(uint8_t msg[], uint8_t len = 3);
        void writeMsg(uint8_t msg);

    private:
        //---- Internal Functions ----
        CommandType readPort();
        CommandType getEventCmdType(byte);
        int8_t getEventDataLength(CommandType);
        void forwardTraffic(byte);
        boolean flt_channel();

        boolean isChannelMessage(CommandType cmd) ;
        boolean isSystemMessage(CommandType cmd) ;

        // Variable length messages
        boolean processSysex(byte newByte );
        boolean processRPN(byte newByte );
        boolean processNRPN(byte newByte );

       

       


        //---- Private Callback Variables ----
        //Voice Messages:
        boolean (*_handleMidiEvent)(const uint8_t , CommandType, Channel, byte[], uint8_t);
        void (*_handleNoteOn)(const uint8_t , Channel, byte note, byte velocity);
        void (*_handleNoteOff)(const uint8_t , Channel, byte note, byte velocity);
        void (*_handleKeyPressure)(const uint8_t , Channel, byte, byte);
        void (*_handleControlChange)(const uint8_t , Channel, byte CC, byte value);
        void (*_handleProgramChange)(const uint8_t , Channel, byte);
        void (*_handleChannelPressure)(const uint8_t , Channel, byte);
        void (*_handlePitchBend)(const uint8_t , Channel, int16_t);
        //Mode Messages:
        void (*_handleChannelMode)(const uint8_t , Channel, byte, byte);
        //System Messages:
        void (*_handleSysEx)(const uint8_t , byte[], unsigned int, byte[], unsigned int);
        void (*_handleQuarterFrame)(const uint8_t , byte, byte);
        void (*_handleSongPosition)(const uint8_t , uint16_t);
        void (*_handleSongSelect)(const uint8_t , byte);
        void (*_handleTuneRequest)(const uint8_t );
        //Real-Time Messages:
        void (*_handleTimingClock)(const uint8_t );
        void (*_handleStart)(const uint8_t );
        void (*_handleContinue)(const uint8_t );
        void (*_handleStop)(const uint8_t );
        void (*_handleActiveSense)(const uint8_t );
        void (*_handleReset)(const uint8_t );
        //---- Main Variables ----
        Stream &_SerialObj;
        byte _listenCh;
        byte _thruMode;
        boolean _sysExMode = false;
        CommandType _previousCmdType;
        Channel _activeChannel;
        boolean _runningStatus = false;

        byte _rawData[16];
        uint8_t _keysPressed;
        int _portID;

        uint8_t msg_length = 0;
        boolean msg_complete;
        boolean msg_discard;
        int8_t msg_pending_bytes;

        byte _msg[3];
        byte _msg_sav[3];  // if interrupted
        byte _msg_lastSent = 0;
        CommandType msg_command;
        Channel msg_channel;

    };

    /** @} */ // end of group1

    /** @defgroup Sessions Classes for Session Management
     *  These Classes are Used For Session Management
     *  @{
     */


 
    /** @} */ // end of group1
} // namespace

// create a function that cann be assiged to a timer interrupt
// provide the name of the instance of the session
#define createTimerInterrupt(Session) \
void _##Session##ISR(void) \
{                          \
	Session.isr();         \
};
// attacha timer interrupt
// provide the name of the instance of the session
#define attachTimerInterrupt(Timer,Session) \
 Timer.attachInterrupt(_##Session##ISR);
 
#endif
