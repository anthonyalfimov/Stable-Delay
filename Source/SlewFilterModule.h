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
//  For the `process` method to be able to handle peak and RMS filtering.

class SlewFilterModule  : public DspModule
{
public:
    SlewFilterModule();

//==============================================================================
    void setState (float riseTimeInMs, float fallTimeInMs);
    void setHold (bool enableHold);

//==============================================================================
    void prepare (double sampleRate, int blockSize) override;

    /**
        Reset the filter state variable (previous output value) to 0.
    */
    void reset() override;

    /**
        Reset the filter state variable (previous output value) to the
        supplied initial value.
    */
    void reset (float initialValue);

    /**
        Process a single value and return a filtered result.

        @Discussion
        NB! If denormals are not disabled on the CPU when this method is called,
        make sure to call `resetDenormals()` afterwards.
    */
    float processSample (float inputValue);

    /**
        Process input samples and place the result in the supplied output location.

        @Discussion
        NB! If denormals are not disabled on the CPU when this method is called,
        make sure to call `resetDenormals()` afterwards.
    */
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

//==============================================================================
    /**
        Reset the filter state variable (previous output value) to zero
        if it is a denormal.

        @Discussion
        If you don't otherwise disable denormals when using this class's
        methods, make sure to call this after every `processSample()` or
        `process()` call.
    */
    void resetDenormals();

private:
    bool mEnableHold = false;
    
//==============================================================================
    float mPreviousValue = 0.0f;

//==============================================================================
    float mRiseTimeInMs = 5.0f;
    float mFallTimeInMs = 50.0f;

    float mRiseWeight = 0.0f;
    float mFallWeight = 0.0f;
    double mWeightExpFactor = getWeightExpFactor();

//==============================================================================
    inline static constexpr float minTimeInMs = 0.05f;

//==============================================================================
    float getAveragingWeight (float transitionTimeInMs) const;
    double getWeightExpFactor() const;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlewFilterModule)
};
