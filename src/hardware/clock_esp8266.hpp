
#define USING_TIM_DIV1

ESP8266TimerInterrupt Timer;
namespace nmidi
{

class Midi_ESP8266Impl : virtual public MidiTimerBase
{
public:

	Midi_ESP8266Impl(): MidiTimerBase() {;};
	Midi_ESP8266Impl(MidiPort *pPortInstance): MidiTimerBase(pPortInstance) {;};

	void init() override __attribute__((always_inline))
	{
		attachInterrupt(isrFunc);
	}
	void start() override __attribute__((always_inline))
	{
		elapsed_ticks = 0;
		eventFunc(START);
		Timer.restartTimer();
	}
	void stop() override __attribute__((always_inline))
	{
		eventFunc(STOP);
		Timer.stopTimer();
	}
	void reset() override __attribute__((always_inline))
	{
		Timer.enableTimer();
	}

protected:
	void setPeriod(long ticks) override
	{
		Timer.attachInterruptInterval(ticks, isrFunc);
	}
	void attachInterrupt(void (*isr)()) override
	{
		Timer.attachInterruptInterval(bpm2Ticks(120), isrFunc);
	}

};
#undef USING_TIM_DIV1
};//Namespace
