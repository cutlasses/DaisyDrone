#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

Oscillator osc;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
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
	hw.StartAudio(AudioCallback);

    //Set up oscillator
    osc.Init( hw.AudioSampleRate() );
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(0.5f);
    osc.SetFreq(440);

	hw.StartAudio(AudioCallback);

	while(1) {}
}

