/*
  ==============================================================================

    RBDDelayModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDAudioConstants.h"

// TODO: unite this with RBDFxPanelStyle, ComboBox index and ID
// TODO: convert to en enum class
enum RBDDelayType
{
    kRBDDelayType_Delay = 0,
    kRBDDelayType_Chorus
};

class DelayModule
{
public:
    DelayModule();
    ~DelayModule();
    
    void setSampleRate (double sampleRate);
    void reset();
    void process (const float* inAudio,
                  float time,
                  float feedback,
                  float wetDry,
                  float type,
                  const float* modulationBuffer,
                  float* outAudio,
                  int numSamplesToRender);

    static constexpr float maxDelayTimeInSeconds = 1.0f;

private:
    float getInterpolatedSample (double delayTimeInSamples) const;
    
    // TODO: Should we initialise smoothed values to the parameter default instead?
    //       They quickly settle on the parameter value - whether it is set to default or the
    //       recalled plugin state. Are there any audible artifacts due to this settling process?
    //       Otherwise, there might not be a reason to change this.
    double mTimeSmoothed = 0.0; // Use member variable to maintain consistent smoothing between blocks
    double mSampleRate = -1.0;
    float mFeedbackSample = 0.0f;

    // TODO: DSP modules per channel -> DSP modules processing vectorised channels
    // The downside of isolating each channel to its own set of DSP modules is that the
    // channels cannot interact without creating dedicated interfaces.

    // TODO: Consider a better data structure for holding the delay buffer
    // JUCE AudioBuffer template should be a good choice. It is designed to hold
    // multiple channels, while our current design handles multiple channesl by creating
    // the appropriate number of DSP modules. Withing the DSP module itself, there's
    // always just one channel. Switching to JUCE AudioBuffer makes sense once we also
    // switch to having a single instance of each DSP module handling vectorised channels

    // std::vector allows reallocation, but is this a real problem for us here?

    std::unique_ptr<float[]> mBuffer;
    int mBufferSize = 0;
    
    int mWritePosition = 0;
    
    // TODO: JUCE DSP modules don't use the Leak Detector. Should I?
    // Probably not?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDDelay)
};

