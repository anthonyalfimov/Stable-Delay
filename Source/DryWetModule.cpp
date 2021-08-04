/*
  ==============================================================================

    DryWetModule.cpp
    Created: 12 Mar 2021 6:09:07pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "DryWetModule.h"

void DryWetModule::setState (float dryWetPercent)
{
    const float wetValue = dryWetPercent / 100.0f;   // convert from %
    const float dryValue = 1.0f - wetValue;

    // Using 3 dB panning rule
    mWetGainSmoothed.setTargetValue (std::sqrt (wetValue));
    mDryGainSmoothed.setTargetValue (std::sqrt (dryValue));
}

void DryWetModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    mDryBuffer = std::make_unique<float[]> (mBlockSize);
    reset();
}

void DryWetModule::reset()
{
    // Clear the dry buffer
    if (mDryBuffer != nullptr)
        FloatVectorOperations::clear (mDryBuffer.get(), mBlockSize);

    // Reset smoothed parameters
    mDryGainSmoothed.reset (mSampleRate, 0.05f);
    mWetGainSmoothed.reset (mSampleRate, 0.05f);
}

void DryWetModule::writeDryBlock (const float* inAudio, int numSamples)
{
    jassert (isPositiveAndNotGreaterThan (numSamples, mBlockSize));

    FloatVectorOperations::copy (mDryBuffer.get(), inAudio, numSamples);
}

void DryWetModule::process (const float* inAudio, float* outAudio,
                            int numSamplesToRender)
{
    jassert (isPositiveAndNotGreaterThan (numSamplesToRender, mBlockSize));

    // If parameters are not smoothing, apply gain to the whole block
    //  Parameter target cannot changing withing a block, so this is safe
    
    if (mDryGainSmoothed.isSmoothing() || mWetGainSmoothed.isSmoothing())
    {
        for (int i = 0; i < numSamplesToRender; ++i)
        {
            outAudio[i] = mDryBuffer[i] * mDryGainSmoothed.getNextValue()
                        + inAudio[i] * mWetGainSmoothed.getNextValue();
        }
    }
    else
    {
        FloatVectorOperations::multiply (mDryBuffer.get(),
                                         mDryGainSmoothed.getTargetValue(),
                                         numSamplesToRender);
        FloatVectorOperations::copyWithMultiply (outAudio, inAudio,
                                                 mWetGainSmoothed.getTargetValue(),
                                                 numSamplesToRender);
        FloatVectorOperations::add (outAudio, mDryBuffer.get(),
                                    numSamplesToRender);
    }
}
