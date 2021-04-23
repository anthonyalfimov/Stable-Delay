/*
  ==============================================================================

    GainModule.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include "GainModule.h"

void GainModule::setState (float gainInDecibels)
{
    // Convert from decibels
    mGainSmoothed.setTargetValue (Decibels::decibelsToGain (gainInDecibels));
}

void GainModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);
    reset();
}

void GainModule::reset()
{
    // Reset smoothed parameters
    mGainSmoothed.reset (mSampleRate, 0.05f);
}

void GainModule::process (const float* inAudio, float* outAudio,
                          int numSamplesToRender)
{
    // If parameter is not smoothing, apply gain to the whole block
    //  Parameter target cannot changing withing a block, so this is safe
    
    if (mGainSmoothed.isSmoothing())
    {
        for (int i = 0; i < numSamplesToRender; ++i)
            outAudio[i] = inAudio[i] * mGainSmoothed.getNextValue();
    }
    else
    {
        FloatVectorOperations::copyWithMultiply (outAudio, inAudio,
                                                 mGainSmoothed.getTargetValue(),
                                                 numSamplesToRender);
    }
}

