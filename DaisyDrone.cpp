#include <array>

#include "daisy_seed.h"
#include "daisysp.h"


using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;

constexpr int NUM_TONES(6);
constexpr int NUM_POTS(NUM_TONES+2);	// tones pots plus 2 additional parameter pots
constexpr int pot_pins[NUM_POTS] = { 17, 18, 19, 20, 21, 22, 16, 15 };

struct ToneSet
{
	float	m_base_frequency;
	char	m_note;
	bool	m_is_sharp;
};

constexpr int NUM_TONE_SETS(12);
ToneSet tones_sets[NUM_TONE_SETS] = {	{55.0f, 'A', false },
										{58.27f, 'A', true },
										{61.74f, 'B', false },
										{65.41f, 'C', false },
										{69.30f, 'C', true },
										{73.42f, 'D', false },
										{77.78f, 'D', true },
										{82.41f, 'E', false },
										{87.31f, 'F', false },
										{92.50f, 'F', true },
										{98.00f, 'G', false },
										{103.83, 'G', true } };

enum class WAVE_SUM_TYPE
{
	AVERAGE,
	SINE_WAVE_FOLD,
	TRIANGLE_WAVE_FOLD,
};

WAVE_SUM_TYPE sum_type = WAVE_SUM_TYPE::AVERAGE;


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

class DroneOscillator
{
	Oscillator		m_low_osc;
	Oscillator		m_base_osc;
	Oscillator		m_high_osc;
	float			m_amplitude = 0.0f;

public:

	void initialise(float sample_rate)
	{
		auto init_osc =[sample_rate](Oscillator& osc)
		{
			osc.Init(sample_rate);
			osc.SetWaveform(osc.WAVE_SIN);
			osc.SetAmp(1.0f);
		};

		init_osc(m_low_osc);
		init_osc(m_base_osc);
		init_osc(m_high_osc);

		m_amplitude				= 0.0f;
	}

	void set_amplitude(float a)
	{
		m_amplitude = a;
	}

	void set_semitone( float base_frequency, int semitone )
	{
		auto semitone_to_frequency = [base_frequency](float semitone)->float
		{
			const float freq_mult	= powf( 2.0f, semitone / 12.0f );
			return base_frequency * freq_mult;
		};

		constexpr int cents = 2;
		constexpr float cent_mult_low = 1.0f - (cents/100.0f);
		constexpr float cent_mult_high = 1.0f - (cents/100.0f);
		m_low_osc.SetFreq( semitone_to_frequency(semitone*cent_mult_low) );
		m_base_osc.SetFreq( semitone_to_frequency(semitone) );
		m_high_osc.SetFreq( semitone_to_frequency(semitone*cent_mult_high) );
	}

	float process()
	{
		const float avg_sin = (m_low_osc.Process() + m_base_osc.Process() + m_high_osc.Process() ) / 3;
		//float avg_sin = m_base_osc.Process();
		return avg_sin * m_amplitude;
	}
};

DroneOscillator oscillators[NUM_TONES];
float gain = 0.0f;

// https://www.desmos.com/calculator/ge2wvg2wgj
float triangular_wave_fold( float in )
{
	const float q_in = in * 0.25f;
	return 4 * (abs(q_in + 0.25f - roundf(q_in+0.25f))-0.25f);
}

float sin_wave_fold( float in )
{
	return sinf(in);
}

void audio_callback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float osc_out = 0.0f;
		for( int o = 0; o < NUM_TONES; ++o )
		{
			osc_out += oscillators[o].process();
		}

		switch(sum_type)
		{
			case WAVE_SUM_TYPE::AVERAGE:
			{
				osc_out /= NUM_TONES;
				break;
			}
			case WAVE_SUM_TYPE::SINE_WAVE_FOLD:
			{
				osc_out = sin_wave_fold(osc_out);
				break;
			}
			case WAVE_SUM_TYPE::TRIANGLE_WAVE_FOLD:
			{
				osc_out = triangular_wave_fold(osc_out);
			}
		}

		osc_out *= gain;
		
		out[0][i] = osc_out;
		out[1][i] = osc_out;
	}
}

void init_adc()
{
	AdcChannelConfig adc_config[NUM_POTS];
	for( int t = 0; t < NUM_POTS; ++t )
	{
		adc_config[t].InitSingle(hw.GetPin(pot_pins[t]));
	}
	hw.adc.Init(adc_config, NUM_POTS);
    hw.adc.Start();
}

void set_tones(float base_frequency)
{
	constexpr int NUM_INTERVALS(4);
	const int intervals[NUM_INTERVALS] = { 12, 3, 4, 5 };	// ocatave, 3rd, 7th, octave
	int interval = 0;
	int semitone = 0;
	for( int t = 0; t < NUM_TONES; ++t )
	{
		oscillators[t].set_semitone(base_frequency, semitone);

		semitone				+= intervals[interval];
		interval				= ( interval + 1 ) % NUM_INTERVALS;
	}
}

int main(void)
{
	hw.Configure();
	hw.Init();

	init_adc();

    //Set up oscillators
	const float sample_rate = hw.AudioSampleRate();
	for( DroneOscillator& osc : oscillators )
	{
		osc.initialise(sample_rate);
	}

	//const float base_frequency = 65.41f; // C2
	const float base_frequency(440);
	set_tones(base_frequency);

	// NOTE: AGND and DGND must be connected for audio and ADC to work
	hw.StartAudio(audio_callback);

	SevenSegmentDisplay seven_seg( { D24, D25, D27, D28, D29, D23, D30, D26 } );
	int current_tone_set = 0;
	const ToneSet& tone_set = tones_sets[current_tone_set];
	seven_seg.set_character( tone_set.m_note );
	seven_seg.set_dot( tone_set.m_is_sharp );
	set_tones(tone_set.m_base_frequency);

	Switch sum_avg_switch;
	Switch sum_sin_switch;
	Switch sum_tri_switch;

	sum_avg_switch.Init(D2);
	sum_sin_switch.Init(D1);
	sum_tri_switch.Init(D0);

	Encoder encoder;
	encoder.Init(D6,D5,D3);

	while(1)
	{	
		for( int t = 0; t < NUM_TONES; ++t )
		{
			const float pot_val = hw.adc.GetFloat(t);
			oscillators[t].set_amplitude( pot_val );
		}

		//const float pot1_val = hw.adc.GetFloat(NUM_TONES);
		gain = hw.adc.GetFloat(NUM_TONES+1);

		sum_avg_switch.Debounce();
		sum_sin_switch.Debounce();
		sum_tri_switch.Debounce();
		if( sum_avg_switch.Pressed() )
		{
			sum_type = WAVE_SUM_TYPE::AVERAGE;
		}
		else if( sum_sin_switch.Pressed() )
		{
			sum_type = WAVE_SUM_TYPE::SINE_WAVE_FOLD;
		}
		else if( sum_tri_switch.Pressed() )
		{
			sum_type = WAVE_SUM_TYPE::TRIANGLE_WAVE_FOLD;
		}

		// use encoder to update tone set
		encoder.Debounce();
		const int inc = encoder.Increment();

		current_tone_set += inc;

		if( current_tone_set > 0 )
		{
			current_tone_set = current_tone_set % NUM_TONE_SETS;
		}
		else if( current_tone_set < 0 )
		{
			current_tone_set = abs(current_tone_set) % NUM_TONE_SETS;
			current_tone_set = NUM_TONE_SETS - current_tone_set;
		}

		if( inc != 0)
		{
			const ToneSet& tone_set = tones_sets[current_tone_set];
			seven_seg.set_character( tone_set.m_note );
			seven_seg.set_dot( tone_set.m_is_sharp );

			set_tones(tone_set.m_base_frequency);
		}

        //wait 1 ms
        System::Delay(1);		
	}
}

