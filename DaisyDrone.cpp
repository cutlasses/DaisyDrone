#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

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

int main(void)
{
	hw.Configure();
	hw.Init();

	hw.StartLog(true); // block until serial connection opened

    Led led1;
    //Initialize led1. We'll plug it into pin 28.
    //false here indicates the value is uninverted
    led1.Init(hw.GetPin(28), false);	

	// setup pots
    AdcChannelConfig adcConfig;
    adcConfig.InitSingle(hw.GetPin(15));
    hw.adc.Init(&adcConfig, 1);
    hw.adc.Start();

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
		const float pot_val = hw.adc.GetFloat(0);
		osc.SetAmp(pot_val);
		osc.SetFreq(pot_val*2000.0f);
		hw.PrintLine("pot %f", pot_val);

		led1.Set(pot_val);
		led1.Update();

        //wait 1 ms
        System::Delay(1);		
	}
}

