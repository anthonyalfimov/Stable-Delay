/*
  ==============================================================================

    FxModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"
#include "DelayModule.h"
#include "SaturationModule.h"
#include "LfoModule.h"
#include "Parameters.h"

class FxModule  : public DspModule
{
public:
    FxModule();

//==============================================================================
    void setState (float time, float feedback, float type,
                   float modRate, float modDepth, float stereoWidth,
                   bool shouldOffsetModulation);
    
//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
//==============================================================================
    // Parameters
    // MARK: Precision: time smoothing
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> mTimeSmoothed;
    SmoothedValue<float> mFeedbackSmoothed;
    FxType::Index mTypeValue = FxType::Delay;

//==============================================================================
    // FX type constants
    inline static constexpr double chorusCentreTime = 0.006;
    inline static constexpr float chorusTimeAmplitude = 0.005f;
    inline static constexpr double flangerCentreTime = 0.0027;
    inline static constexpr float flangerTimeAmplitude = 0.0023f;
    inline static constexpr float maxDelayTimeAmplitude = 0.01f;
    inline static constexpr float minDelayTimeAmplitude = 0.0005f;

//==============================================================================
    // Drive
    SaturationModule mSaturator;

//==============================================================================
    // Delay
    DelayModule mDelay { RBD::maxDelayTimeInSeconds + maxDelayTimeAmplitude };

//==============================================================================
    // Modulation
    // MARK: Precision: modulation
    LfoModule mLfo;
    std::unique_ptr<float[]> mModulationBuffer;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxModule)
};

