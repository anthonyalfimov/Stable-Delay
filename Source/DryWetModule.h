/*
  ==============================================================================

    DryWetModule.h
    Created: 12 Mar 2021 6:09:07pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"

class DryWetModule  : public DspModule
{
public:
    DryWetModule() = default;

//==============================================================================
    void setState (float dryWetPercent);
    
//==============================================================================

    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void writeDryBlock (const float* inAudio, int numSamples);
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
    SmoothedValue<float> mDryGainSmoothed;
    SmoothedValue<float> mWetGainSmoothed;

//==============================================================================
    std::unique_ptr<float[]> mDryBuffer;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DryWetModule)
};
