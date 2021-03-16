/*
  ==============================================================================

    RBDDelayModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
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
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

    void setState (float time, float feedback, float type,
                   float modRate, float modDepth, float stereoWidth,
                   bool shouldOffsetModulation);

private:
    //==========================================================================
    // Parameters
    // TODO: Is there actual benefit from type double for delay time?
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> mTimeSmoothed;
    SmoothedValue<float> mFeedbackSmoothed;
    FxType::Index mTypeValue = FxType::Delay;

    //==========================================================================
    // FX type constants
    inline static const double chorusCentreTime = 0.006;
    inline static const float chorusTimeAmplitude = 0.005f;
    inline static const double flangerCentreTime = 0.0027;
    inline static const float flangerTimeAmplitude = 0.0023f;
    inline static const float maxDelayTimeAmplitude = 0.01f;
    inline static const float minDelayTimeAmplitude = 0.0005f;

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

