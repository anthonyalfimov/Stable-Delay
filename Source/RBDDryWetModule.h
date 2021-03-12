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
    void pushDryBlock (const float* inAudio, int numSamples);
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;
    void reset() override;

    void setState (float dryWetPercent);

private:
    std::unique_ptr<float[]> mDryBuffer;
    
    float mDryWetValue = 0.0f;
};
