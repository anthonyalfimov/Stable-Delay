/*
  ==============================================================================

    RBDDelayModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDDspModule.h"
#include "RBDLfoModule.h"
#include "RBDSaturationModule.h"
#include "RBDParameters.h"

class DelayModule  : public DspModule
{
public:
    DelayModule();
    ~DelayModule();

    void prepare (double sampleRate, int blockSize) override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;
    void reset() override;

    void setState (float time, float feedback, float type,
                   float modRate, float modDepth, float modOffset);

private:
    float mTimeValue = 0.0f;
    float mFeedbackValue = 0.0f;
    FxType::Index mTypeValue = FxType::Delay;

    double mTimeSmoothed = 0.0;

    //==========================================================================
    // Delay

    // TODO: Consider a better data structure for holding the delay buffer
    //  JUCE AudioBuffer template should be a good choice. It is designed to hold
    //  multiple channels, while our current design handles multiple channesl by creating
    //  the appropriate number of DSP modules. Withing the DSP module itself, there's
    //  always just one channel. Switching to JUCE AudioBuffer makes sense once we also
    //  switch to having a single instance of each DSP module handling vectorised channels

    //  std::vector allows reallocation, but is this a real problem for us here?

    std::unique_ptr<float[]> mAudioBuffer;
    int mAudioBufferSize = 0;
    
    int mWritePosition = 0;
    float mFeedbackSample = 0.0f;

    float getInterpolatedSample (double delayTimeInSamples) const;

    //==========================================================================
    // LFO
    LfoModule mLfo;
    std::unique_ptr<float[]> mModulationBuffer;

    //==========================================================================
    // Saturation
    SaturationModule mSaturator;
};

