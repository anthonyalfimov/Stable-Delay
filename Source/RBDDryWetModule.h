/*
  ==============================================================================

    RBDDryWetModule.h
    Created: 12 Mar 2021 6:09:07pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDDspModule.h"

class DryWetModule  : public DspModule
{
public:
    DryWetModule();
    ~DryWetModule();

    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void pushDryBlock (const float* inAudio, int numSamples);
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

    void setState (float dryWetPercent);

private:
    SmoothedValue<float> mDryGainSmoothed;
    SmoothedValue<float> mWetGainSmoothed;

    std::unique_ptr<float[]> mDryBuffer;
};
