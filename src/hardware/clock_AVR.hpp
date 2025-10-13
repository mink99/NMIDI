
namespace nmidi
{

class Midi_AVRImpl : virtual public MidiTimerBase
{
public:
	TimerOne Timer1;
	Midi_AVRImpl(): nmidi::MidiTimerBase() {;};
	Midi_AVRImpl(nmidi::MidiPort *pPortInstance): MidiTimerBase(pPortInstance) {;};

	void init() override __attribute__((always_inline))
	{
		Timer1.initialize(bpm2Ticks(120)); // shoud be 120BPM
		attachInterrupt(isrFunc);
	}
	void start() override __attribute__((always_inline))
	{
		elapsed_ticks = 0;
		eventFunc(START);
		Timer1.start();
	}
	void stop() override __attribute__((always_inline))
	{
		eventFunc(STOP);
		Timer1.stop();
	}
	void reset() override __attribute__((always_inline))
	{
		Timer1.restart();
	}

protected:
	void setPeriod(long ticks) override
	{
		Timer1.setPeriod(ticks);
	}
	void attachInterrupt(void (*isr)()) override
	{
		Timer1.attachInterrupt(isr);
	}

};
};//Namespace