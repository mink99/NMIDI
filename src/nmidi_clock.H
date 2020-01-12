namespace nmidi
{
	    /** @brief A Session for Midi Clock */


#define mode_none 0
#define mode_stop 0
#define mode_run 1
#define mode_armed 8



    class MidiClockSession
    {
    public:
        MidiClockSession(  MidiPort *);
        void continuous(boolean);
        void begin(uint8_t, boolean );
        void start();
        void stop();
        void cont();
        void reset();
        long ticks();
        long beats();
        void bpm(uint8_t);
        void locate(int16_t);
        long bpmToTick();
        long bpmToTick(uint8_t);
        void isr();
        uint8_t bpm();
		
    private:
        MidiPort *_midiPort;
        volatile long _counter;
        volatile long _beats;
        volatile uint8_t _bpm = 120;
        boolean _continuous;
        volatile int _mode = mode_stop;
        int16_t _cpt = 100;
		boolean isShuffleTrigger (int8_t _tick, int8_t _shift = 0, int8_t _res = 8);
    };
}
 
 