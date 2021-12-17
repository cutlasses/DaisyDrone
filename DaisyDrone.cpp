#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

const int NUM_TONES(2);
const int pot_pins[NUM_TONES] = { 15, 16 };
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
	hw.PrintLine("Base Frequency %d", int(base_frequency * 100));

	const int semitones[] = { 0, 3, 7, 12 };
	
	for( int t = 0; t < NUM_TONES; ++t )
	{
		const int semitone = semitones[t];

		const float freq_mult	= powf( 2.0f, semitone / 12.0f );
		const float freq		= base_frequency * freq_mult;
		oscillators[t].SetFreq(freq);

		hw.PrintLine("%d %d %d", semitone, int(freq_mult * 100), int(freq * 100));
	}
}

int main(void)
{
	hw.Configure();
	hw.Init();

	hw.StartLog(false/*block until serial connection opened*/);

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
	float base_frequency = 440.0f;
	set_tones(base_frequency);

	// NOTE: AGND and DGND must be connected for audio and ADC to work
	hw.StartAudio(audio_callback);

	hw.PrintLine("Startup complete");

	while(1)
	{	
		for( int t = 0; t < NUM_TONES; ++t )
		{
			const float pot_val = hw.adc.GetFloat(t);

			oscillators[t].SetAmp(pot_val);
		}

        //wait 1 ms
        System::Delay(1);		
	}
}

