/*
  ==============================================================================

    SlewFilterModule.h
    Created: 12 Aug 2021 2:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"

// TODO: Consider passing lambdas to apply to the sample pre and post processing
//  for the `process` method

class SlewFilterModule  : public DspModule
{
public:
    SlewFilterModule();

//==============================================================================
    void setState (float riseTimeInMs, float fallTimeInMs);

//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void reset (float initialValue);

    float processSample (float inputValue);
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
    float mPreviousValue = 0.0f;

//==============================================================================
    float mRiseTimeInMs = 5.0f;
    float mFallTimeInMs = 50.0f;

    float mRiseWeight = 0.0f;
    float mFallWeight = 0.0f;
    double mWeightExpFactor = getWeightExpFactor();

//==============================================================================
    // TODO: Confirm that this allows passing 20 kHz sine
    // Set minimum time constant to a 20 kHz period duration
    inline static constexpr float minTimeInMs = 0.05f;

//==============================================================================
    float getAveragingWeight (float transitionTimeInMs) const;
    double getWeightExpFactor() const;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlewFilterModule)
};
