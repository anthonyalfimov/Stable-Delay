/*
  ==============================================================================

    SlewFilterModule.cpp
    Created: 12 Aug 2021 2:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "SlewFilterModule.h"

SlewFilterModule::SlewFilterModule()
{
    setState (mRiseTimeInMs, mFallTimeInMs);   // calculate the weights
}

//==============================================================================
void SlewFilterModule::setState (float riseTimeInMs, float fallTimeInMs)
{
    mRiseTimeInMs = riseTimeInMs;
    mFallTimeInMs = fallTimeInMs;

    mRiseWeight = getAveragingWeight (mRiseTimeInMs);
    mFallWeight = getAveragingWeight (mFallTimeInMs);
}

//==============================================================================
void SlewFilterModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);
    mWeightExpFactor = getWeightExpFactor();
    setState (mRiseTimeInMs, mFallTimeInMs);   // recalculate the weights
    reset();
}

void SlewFilterModule::reset()
{
    reset (0.0f);
}

void SlewFilterModule::reset (float initialValue)
{
    mPreviousValue = initialValue;
}

float SlewFilterModule::processSample (float inputValue)
{
    float weight = (inputValue > mPreviousValue) ? mRiseWeight : mFallWeight;
    float result = inputValue + weight * (mPreviousValue - inputValue);
    mPreviousValue = result;

    return result;
}

void SlewFilterModule::process (const float* inAudio, float* outAudio,
                                int numSamplesToRender)
{
    for (int i = 0; i < numSamplesToRender; ++i)
        outAudio[i] = processSample (inAudio[i]);
}

//==============================================================================
void SlewFilterModule::resetDenormals()
{
    JUCE_SNAP_TO_ZERO (mPreviousValue);
}

//==============================================================================
float SlewFilterModule::getAveragingWeight (float transitionTimeInMs) const
{
    return (transitionTimeInMs < minTimeInMs) ? 0.0f
                    : std::exp (mWeightExpFactor / transitionTimeInMs);
}

double SlewFilterModule::getWeightExpFactor() const
{
    // The weight coefficient is calculated to cover 86% of the initial delta
    //  to the target value in the required transition time (in milliseconds)
    return -2.0 * 1000.0 / mSampleRate;
}
