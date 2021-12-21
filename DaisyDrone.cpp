#include <array>

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

const int NUM_TONES(5);
const int pot_pins[NUM_TONES] = { 15, 16, 17, 18, 19 };
Oscillator oscillators[NUM_TONES];


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

	hw.StartLog(true/*block until serial connection opened*/);

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

/*
	uint8_t pins[] = { 30, 29, 27, 26 };
	for( uint8_t pin : pins )
	{
		dsy_gpio_pin gpio_pin = { DSY_GPIOA, pin };
		dsy_gpio gpio = { gpio_pin, DSY_GPIO_MODE_INPUT, DSY_GPIO_NOPULL };
		dsy_gpio_write( &gpio, true );
	}*/

	hw.PrintLine("Startup complete");

	/*
	oscillators[0].SetFreq(80);
	oscillators[1].SetFreq(160);
	oscillators[2].SetFreq(320);
	oscillators[3].SetFreq(640);
	oscillators[4].SetFreq(1280);*/

	while(1)
	{	
		//hw.Print("pots ");
		for( int t = 0; t < NUM_TONES; ++t )
		{
			const float pot_val = hw.adc.GetFloat(t);

			//hw.Print("%d ", int(pot_val*100));

			oscillators[t].SetAmp(pot_val);
		}
		//hw.PrintLine("");

        //wait 1 ms
        System::Delay(1);		
	}
}

