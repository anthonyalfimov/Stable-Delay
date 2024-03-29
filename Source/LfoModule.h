/*
  ==============================================================================

    LfoModule.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"

class LfoModule  : public DspModule
{
public:
    LfoModule() = default;

//==============================================================================
    void setState (float rate, float amplitude, float phaseOffsetPercent);

//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inBuffer, float* outBuffer,
                  int numSamplesToRender) override;

private:
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> mRateSmoothed;
    SmoothedValue<float> mAmplitudeSmoothed;
    SmoothedValue<float> mPhaseOffsetSmoothed;

//==============================================================================
    float mPhase = 0.0f;
    float mHarmonicPhase = 0.0f;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoModule)
};
