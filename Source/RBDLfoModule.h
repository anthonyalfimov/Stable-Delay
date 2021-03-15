/*
  ==============================================================================

    RBDLfoModule.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDDspModule.h"

class LfoModule  : public DspModule
{
public:
    LfoModule();
    ~LfoModule();

    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inBuffer, float* outBuffer,
                  int numSamplesToRender) override;

    void setState (float rate, float amplitude, float phaseOffsetPercent);
    
private:
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> mRateSmoothed;
    SmoothedValue<float> mAmplitudeSmoothed;
    SmoothedValue<float> mPhaseOffsetSmoothed;

    float mPhase = 0.0f;
};
