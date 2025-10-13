

namespace nmidi
{

class Midi_DUEImpl : virtual public MidiTimerBase
{
public:
	Midi_DUEImpl(): MidiTimerBase() {;};
	Midi_DUEImpl(MidiPort *pPortInstance): MidiTimerBase(pPortInstance) {;};

	void init() override __attribute__((always_inline))
	{
		// Timer0.initialize(bpmToTicks(120)); // shoud be 120BPM
		attachInterrupt(isrFunc);
	}
	void start() override __attribute__((always_inline))
	{
		elapsed_ticks = 0;
		eventFunc(START);
		Timer0.start();
	}
	void stop() override __attribute__((always_inline))
	{
		eventFunc(STOP);
		Timer0.stop();
	}
	void reset() override __attribute__((always_inline))
	{
		Timer0.stop();
	}

protected:
	void setPeriod(long ticks) override
	{
		Timer0.setPeriod(ticks);
	}
	void attachInterrupt(void (*isr)()) override
	{
		Timer0.attachInterrupt(isr);
	}

};
};//Namespace
