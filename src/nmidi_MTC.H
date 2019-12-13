namespace nmidi
{
   /** @brief A Session for Midi Time Code  */
    class MTCSession
    {
    public:
        MTCSession(  MidiPort *);
        void start();
        void stop();
        void cont();
        void reset();
        void fps(MTCFrames);  // 24,25,29,30
        void locate(int8_t, int8_t, int8_t, int8_t);
        
        void isr();
        MTCFrames fps();
        void sendMTCQuarterMessage();
		void sendMTCFullFrameMessage();
    private:
		long microsPerTick();
		long microsPerTick( MTCFrames);
        MidiPort *_midiPort;
        byte _hh, _mm, _ss, _ff, _frames;
        MTCFrames _fps = MTC_25FPS;
        int16_t _mpt = 100;
        int8_t  _ctr = 0;
    };
}