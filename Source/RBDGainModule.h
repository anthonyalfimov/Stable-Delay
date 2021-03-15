/*
  ==============================================================================

    RBDGainModule.h
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDDspModule.h"

class GainModule  : public DspModule
{
public:
    // TODO: Is there a reason to explicitely provide empty ctor and dtor?

    GainModule();
    ~GainModule();

    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

    void setState (float gainInDecibels);

private:
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> mGainSmoothed;
};

