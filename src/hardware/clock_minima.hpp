
FspTimer fspTimer;

void fsptimer_callback(timer_callback_args_t __attribute((unused)) *p_args)
{
	nmidi::isrFunc();
}
namespace nmidi
{

class Midi_UNOR4Impl : virtual public MidiTimerBase
{
public:
	Midi_UNOR4Impl(): MidiTimerBase() { ;};
	Midi_UNOR4Impl(MidiPort *pPortInstance): MidiTimerBase(pPortInstance) {;};


	void init() override __attribute__((always_inline))
	{
		int result = 0;
		float rate = 120;
		uint8_t timer_type = GPT_TIMER;
		int8_t tindex = FspTimer::get_available_timer(timer_type);
		if (tindex < 0)
		{
			tindex = FspTimer::get_available_timer(timer_type, true);
		}
		if (tindex < 0)
		{
			Serial.println("get_available_timer fail");
			return ;//-1;
		}

		FspTimer::force_use_of_pwm_reserved_timer();

		if (!fspTimer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, rate, 0.0f, fsptimer_callback))
		{
			Serial.println("fspTimer.begin fail");
			return ;// -2;
		}

		if (!fspTimer.setup_overflow_irq())
		{
			Serial.println("fspTimer.setup_overflow_irq fail");
			return ;// -3;
		}

		if (!fspTimer.open())
		{
			Serial.println("fspTimer.open fail");
			return ;// -4;
		}
		return ;// true;
	}

	void start() override __attribute__((always_inline))
	{
		elapsed_ticks = 0;
		eventFunc(START);
		fspTimer.start();

	}
	void stop() override __attribute__((always_inline))
	{

		eventFunc(STOP);
		fspTimer.stop();

	}
	void reset() override __attribute__((always_inline))
	{
		fspTimer.reset();

	}
	void setBPM(uint8_t __bpm)
	{
		// Serial.print(__bpm) ;
		// Serial.println("      fspTimer-setBPM") ;
		_bpm = __bpm;
		setPeriod(bpm2Ticks(_bpm));
	}
protected:
	void setPeriod(long ticks) override
	{
		// Serial.print(ticks) ;
		// Serial.println("     fspTimer-setPeriod try") ;
		
		fspTimer.set_period_ms(ticks);
		fspTimer.start();
		// Serial.println("fspTimer-setPeriod ") ;

	}
	void attachInterrupt(void (*isr)()) override
	{
		Serial.println("fspTimer try") ;
		fspTimer.set_irq_callback(fsptimer_callback,nullptr);
		Serial.println("fspTimer4-attachInterrupt ") ;
	}

};
};//Namespace
