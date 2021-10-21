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
#include "SlewFilterModule.h"
#include "SaturationModule.h"
#include "Parameters.h"

class FxModule  : public DspModule
{
public:
    FxModule();

//==============================================================================
    void setState (float driveInDecibels, float clipRise, float clipFall);
    
//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
//==============================================================================
    // Drive
    SmoothedValue<float> mDriveSmoothed;
    SlewFilterModule mDetector;
    SaturationModule mSaturator;

    inline static constexpr float clippingThreshold = 8.0f;
    inline static constexpr float postCutFactor = 0.65f; // cut for ~equal loudness

    inline static constexpr float maxThreshold = 5.0f;
    inline static constexpr float minThreshold = -72.0f;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxModule)
};

