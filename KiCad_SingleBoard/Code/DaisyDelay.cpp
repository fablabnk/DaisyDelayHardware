#include "daisysp.h"
#include "daisy_seed.h"
// #include <led.h>

// Interleaved audio definitions
#define LEFT (i)
#define RIGHT (i + 1)

// Set max delay time to 0.75 of samplerate.
#define MAX_DELAY static_cast<size_t>(48000 * 0.75f)

using namespace daisysp;
using namespace daisy;

static DaisySeed hw;

// Declare a DelayLine of MAX_DELAY number of floats.
static DelayLine<float, MAX_DELAY> del;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    float osc_out, env_out, feedback, del_out, sig_out;
    for(size_t i = 0; i < size; i += 2)
    {
        // Parameter 1: Delay Time (secs)
		float delayTimeParam = hw.adc.GetFloat(0);
		// float clampedDelayTimeParam = clamp(delayTimeParam, 0.f, 1.f);

		// Choose min and max delay times based on position of switch
        switch.Debounce();

		if (switch.Pressed())
		{
			led.Set(1.0f);
			minDelayTimeSecs = MIN_SHORT_DELAY_TIME;
			maxDelayTimeSecs = MAX_SHORT_DELAY_TIME;
		}
		else
		{
			led.Set(0.0f));
			minDelayTimeSecs = MIN_LONG_DELAY_TIME;
			maxDelayTimeSecs = MAX_LONG_DELAY_TIME;
		}
		
		float rangeDelayTimeSecs = maxDelayTimeSecs - minDelayTimeSecs;
		float delayTime = (clampedDelayTimeParam * rangeDelayTimeSecs) + minDelayTimeSecs;

        // smoothing quick changes between delay times
		float smoothedDelayTime;
		float coeff = 1.0f / (2.5f * args.sampleRate);
		daisysp::fonepole(smoothedDelayTime, delayTime, coeff);
		
		del.SetDelay(args.sampleRate * delayTime);

        del_out = del.Read();
        sig_out  = del_out + in[LEFT];

		// Parameter 2: Feedback
		float feedbackParam = hw.adc.GetFloat(1);
		// float clampedFeedbackParam = clamp(feedbackParam, 0.f, 1.f);
        feedback = (del_out * feedbackParam * MAX_FEEDBACK) + in[LEFT];
        del.Write(feedback);

		outputs[LEFT] = sig_out;
    }
}

int main(void)
{
    // initialize seed hardware and daisysp modules
    float sample_rate;
    hw.Configure();
    hw.Init();

    AdcChannelConfig adcConfig;
    adcConfig.InitSingle(hw.GetPin(22));
    adcConfig.InitSingle(hw.GetPin(23));

    Switch switchy;
    switchy.Init(hw.GetPin(12), 1000);

    Led led;
    led.Init(8, 1000);

    hw.adc.Init(&adcConfig, 1);
    hw.adc.Start();

    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();
    del.Init();

    // Set Delay time to 0.75 seconds
    del.SetDelay(sample_rate * 0.75f);

    // start callback
    hw.StartAudio(AudioCallback);

    while(1) {}
}