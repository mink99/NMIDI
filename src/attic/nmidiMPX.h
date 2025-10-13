namespace nmidi
{
	class MidiMPX
	{
	public:
		MidiMPX (Stream *_s)
		{
			use_DirectPortManipulation = true;
			// use Pin 49,48,47,46 -> PortL  on 2560
			// use Pin 8,9,10,11   -> PortB  on UNO etc...
	#if defined(__AVR_ATmega328P__)
	#define DPMPORT PORTB
	#pragma message "USING UNO/NANO (ATmega328P) on PORTB"
			pin0 = 8;
			pin1 = 9;
			pin2 = 10;
			pinEnable = 11;
	#elif defined(__AVR_ATmega2560__)
	#define DPMPORT PORTL
	#pragma message "USING MEGA on PORTL"
			pin0 = 49;
			pin1 = 48;
			pin2 = 47;
			pinEnable = 46;
	#else
	#warning board must be either UNO or Mega, no Direct Port Manipulation
	#define DPMPORT NULL
			use_DirectPortManipulation = false;
			pin0 = 8;
			pin1 = 9;
			pin2 = 10;
			pinEnable = 11;
	#endif
			s = _s;

			pinMode(pin0, OUTPUT);
			pinMode(pin1, OUTPUT);
			pinMode(pin2, OUTPUT);
			//-------------------------------------------
			pinMode(pinEnable, OUTPUT);
			//-------------------------------------------
			digitalWrite(pin0, LOW);
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, LOW);
			digitalWrite(pinEnable, LOW);

		}
		MidiMPX (int p0, int p1, int p2, int pEnable, Stream *_s)
		{
			use_DirectPortManipulation = false;
			pin0 = p0;
			pin1 = p1;
			pin2 = p2;
			pinEnable = pEnable;

			s = _s;

			pinMode(pin0, OUTPUT);
			pinMode(pin1, OUTPUT);
			pinMode(pin2, OUTPUT);
			//-------------------------------------------
			pinMode(pinEnable, OUTPUT);
			//-------------------------------------------
			digitalWrite(pin0, LOW);
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, LOW);
			digitalWrite(pinEnable, LOW);

		}
		/**
		 *  \brief Switch the Serial output to a 4051 output
		 *  
		 *  \param [in] the 4051 output
		 *  \return Return nothing
		 *  
		 *  \details will run in either Direct Portmanipulation or bit by bit, depending on the constructor
		 */
		void enable(int nOut)
		{

			if (_hw == nOut) return;
			_hw = nOut;
			if (use_DirectPortManipulation)
			{
				s->flush();
				// save to tmp
				int8_t t1 = DPMPORT ; // = dpmPort;
				// clear out bit 0..3
				t1 &= 0b11110000;
				/// switch bits
				t1 |= switchBits_On[nOut];
				// print port on, as this is one op, no enable - disable
				DPMPORT = t1;
			}
			else
			{
				int r0 = bitRead(nOut, 0);   // use this with arduino 0013 (and newer versions)
				int r1 = bitRead(nOut, 1);   // use this with arduino 0013 (and newer versions)
				int r2 = bitRead(nOut, 2);   // use this with arduino 0013 (and newer versions)
				s->flush();
				digitalWrite(pinEnable, HIGH);
				digitalWrite(pin0, r0);
				digitalWrite(pin1, r1);
				digitalWrite(pin2, r2);
				digitalWrite(pinEnable, LOW);
			}
		}
		int getHW()
		{
			return _hw;
		}
	protected:
		boolean use_DirectPortManipulation = false;
		int8_t _hw = -1;

		volatile uint8_t *dpmPort;

		uint8_t pin0;
		uint8_t pin1;
		uint8_t pin2;
		uint8_t pinEnable;
		Stream *s;
	// bits 0..3 is the 4051 registers, 4 is 4051 enable
		int8_t switchBits_On[8] =
		{
			0b00000000, //1
			0b00000001, //2
			0b00000010, //3
			0b00000011, //4
			0b00000100, //5
			0b00000101, //6
			0b00000110, //7
			0b00000111, //8
		};
		int8_t switchBits_Off[8] =
		{
			0b00001000, //1
			0b00001001, //2
			0b00001010, //3
			0b00001011, //4
			0b00001100, //5
			0b00001101, //6
			0b00001110, //7
			0b00001111, //8
		};




	};
}
