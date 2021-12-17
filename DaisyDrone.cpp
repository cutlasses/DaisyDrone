#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

const int NUM_POTS(2);
const int pot_pins[NUM_POTS] = { 15, 16 };

DaisySeed hw;

Oscillator osc;

void audio_callback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		const float osc_out = osc.Process();

		out[0][i] = osc_out;
		out[1][i] = osc_out;
	}
}

void init_pots()
{
	AdcChannelConfig adc_config[NUM_POTS];
	for( int p = 0; p < NUM_POTS; ++p )
	{
		adc_config[p].InitSingle(hw.GetPin(pot_pins[p]));
	}
	hw.adc.Init(adc_config, NUM_POTS);
    hw.adc.Start();
}

int main(void)
{
	hw.Configure();
	hw.Init();

	hw.StartLog(false/*block until serial connection opened*/);

    Led led1;
    //Initialize led1. We'll plug it into pin 28.
    //false here indicates the value is uninverted
    led1.Init(hw.GetPin(28), false);	

	init_pots();

    //Set up oscillator
    osc.Init( hw.AudioSampleRate() );
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(0.5f);
    osc.SetFreq(440);

	// NOTE: AGND and DGND must be connected for audio and ADC to work
	hw.StartAudio(audio_callback);

	hw.PrintLine("Startup complete");

	while(1)
	{	
		for( int p = 0; p < NUM_POTS; ++p )
		{
			const int pot_val = hw.adc.GetFloat(p) * 1000.0f;
			hw.Print("\t%d", pot_val);
		}
		hw.PrintLine("");

		const float pot_val = hw.adc.GetFloat(0);
		osc.SetAmp(pot_val);
		osc.SetFreq(pot_val*2000.0f);

		led1.Set(pot_val);
		led1.Update();

        //wait 1 ms
        System::Delay(1);		
	}
}

