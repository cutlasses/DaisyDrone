#include <array>

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;

constexpr int NUM_TONES(5);
constexpr int pot_pins[NUM_TONES] = { 15, 16, 17, 18, 19 };
Oscillator oscillators[NUM_TONES];

struct ToneSet
{
	float	m_base_frequency;
	char	m_note;
	bool	m_is_sharp;
};

constexpr int NUM_TONE_SETS(12);
ToneSet tones_sets[NUM_TONE_SETS] = {	{0.0f, 'A', false },
										{0.0f, 'A', true },
										{0.0f, 'B', false },
										{0.0f, 'C', false },
										{0.0f, 'C', true },
										{0.0f, 'D', false },
										{0.0f, 'D', true },
										{0.0f, 'E', false },
										{0.0f, 'F', false },
										{0.0f, 'F', true },
										{0.0f, 'G', false },
										{0.0f, 'G', true } };


class SevenSegmentDisplay
{
	static constexpr int NUM_LEDS = 8;

	std::array<GPIO, NUM_LEDS>		m_gpio_pins;

public:

	// Pins a,b,c,d,e,f,g,DP
	SevenSegmentDisplay( const std::array<Pin, NUM_LEDS>& pins )
	{
		for( int p = 0; p < NUM_LEDS; ++p )
		{
			m_gpio_pins[p].Init(pins[p], GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
		}

		// turn the leds off
		for( GPIO& gpio : m_gpio_pins )
		{
			gpio.Write(false);
		}	
	}

	void set_byte_mask( char mask )
	{
		int bit = 1;
		for( int p = 0; p < (NUM_LEDS-1); ++p ) // NUM_LEDS-1 because last led is the dot
		{
			GPIO& gpio = m_gpio_pins[p];
			if( mask & bit )
			{
				gpio.Write(true);
			}
			else
			{
				gpio.Write(false);
			}

			bit <<= 1;
		}
	}

	void set_character( char character )
	{
		switch(character)
		{
			case 'A':
			{
				set_byte_mask(0b01110111);
				break;
			}
			case 'B':
			{
				set_byte_mask(0b01111111);
				break;
			}
			case 'C':
			{
				set_byte_mask(0b00111001);
				break;
			}
			case 'D':
			{
				set_byte_mask(0b00111111);
				break;
			}
			case 'E':
			{
				set_byte_mask(0b01111001);
				break;
			}
			case 'F':
			{
				set_byte_mask(0b01110001);
				break;
			}
			case 'G':
			{
				set_byte_mask(0b01111101);
				break;
			}				
			default:
			{
				set_byte_mask(0);
			}
		}
	}

	void set_dot( bool set )
	{
		m_gpio_pins.back().Write(set);
	}
};


void audio_callback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float osc_out = 0.0f;
		for( int o = 0; o < NUM_TONES; ++o )
		{
			osc_out += oscillators[o].Process();
		}
		osc_out /= NUM_TONES;

		out[0][i] = osc_out;
		out[1][i] = osc_out;
	}
}

void init_adc()
{
	AdcChannelConfig adc_config[NUM_TONES];
	for( int t = 0; t < NUM_TONES; ++t )
	{
		adc_config[t].InitSingle(hw.GetPin(pot_pins[t]));
	}
	hw.adc.Init(adc_config, NUM_TONES);
    hw.adc.Start();
}

void set_tones(float base_frequency)
{
	constexpr int NUM_INTERVALS(3);
	const int intervals[NUM_INTERVALS] = { 3, 4, 5 };	// 3rd, 7th, octave
	int interval = 0;
	int semitone = 0;
	for( int t = 0; t < NUM_TONES; ++t )
	{
		const float freq_mult	= powf( 2.0f, semitone / 12.0f );
		const float freq		= base_frequency * freq_mult;
		oscillators[t].SetFreq(freq);

		hw.PrintLine("%d %d %d %d", semitone, interval, int(freq_mult * 100), int(freq * 100));

		semitone				+= intervals[interval];
		interval				= ( interval + 1 ) % NUM_INTERVALS;
	}

	/*
	hw.PrintLine("Base Frequency %d", int(base_frequency * 100));

	const int semitones[] = { 0, 3, 7, 12 };
	
	for( int t = 0; t < NUM_TONES; ++t )
	{
		const int semitone		= semitones[t];

		const float freq_mult	= powf( 2.0f, semitone / 12.0f );
		const float freq		= base_frequency * freq_mult;
		oscillators[t].SetFreq(freq);

		hw.PrintLine("%d %d %d", semitone, int(freq_mult * 100), int(freq * 100));
	}
	*/
}

int main(void)
{
	hw.Configure();
	hw.Init();

	init_adc();

    //Set up oscillators
	const float sample_rate = hw.AudioSampleRate();
	for( Oscillator& osc : oscillators )
	{
		osc.Init(sample_rate);
		osc.SetWaveform(osc.WAVE_SIN);
		osc.SetAmp(0.0f);
	}

	//const float base_frequency = 65.41f; // C2
	const float base_frequency(440);
	set_tones(base_frequency);

	// NOTE: AGND and DGND must be connected for audio and ADC to work
	hw.StartAudio(audio_callback);

	SevenSegmentDisplay seven_seg( { D24, D25, D27, D29, D30, D23, D22, D26 } );
	int current_tone_set = 0;
	const ToneSet& tone_set = tones_sets[current_tone_set];
	seven_seg.set_character( tone_set.m_note );
	seven_seg.set_dot( tone_set.m_is_sharp );

	// button D20
	Switch button;
	button.Init(D20);

	while(1)
	{	
		for( int t = 0; t < NUM_TONES; ++t )
		{
			oscillators[t].SetAmp( hw.adc.GetFloat(t) );
		}

		button.Debounce();

		if( button.FallingEdge() )
		{
			current_tone_set = (current_tone_set + 1) % NUM_TONE_SETS;

			const ToneSet& tone_set = tones_sets[current_tone_set];
			seven_seg.set_character( tone_set.m_note );
			seven_seg.set_dot( tone_set.m_is_sharp );
		}

        //wait 1 ms
        System::Delay(1);		
	}
}

