/*
  ==============================================================================

    RBDDryWetModule.cpp
    Created: 12 Mar 2021 6:09:07pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDDryWetModule.h"

DryWetModule::DryWetModule()
{

}

DryWetModule::~DryWetModule()
{

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

void DryWetModule::pushDryBlock (const float* inAudio, int numSamples)
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

void DryWetModule::setState (float dryWetPercent)
{
    // TODO: Consider using a different mixing rule to retain volume at 50%

    // Linear mixing rule
    const float wetGain = dryWetPercent / 100.0f;   // convert from %
    const float dryGain = 1.0f - wetGain;

    mDryGainSmoothed.setTargetValue (dryGain);
    mWetGainSmoothed.setTargetValue (wetGain);
}
