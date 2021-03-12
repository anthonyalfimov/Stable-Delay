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

void DryWetModule::pushDryBlock (const float* inAudio, int numSamples)
{
    jassert (isPositiveAndNotGreaterThan (numSamples, mBlockSize));

    FloatVectorOperations::copy (mDryBuffer.get(), inAudio, numSamples);
}

void DryWetModule::process (const float* inAudio, float* outAudio,
                            int numSamplesToRender)
{
    jassert (isPositiveAndNotGreaterThan (numSamplesToRender, mBlockSize));

    // TODO: Consider using a different mixing rule

    // Linear mixing rule
    const float wetGain = mDryWetValue;
    const float dryGain = 1.0f - wetGain;

    FloatVectorOperations::multiply (mDryBuffer.get(), dryGain, numSamplesToRender);
    FloatVectorOperations::copyWithMultiply (outAudio, inAudio, wetGain,
                                             numSamplesToRender);
    FloatVectorOperations::add (outAudio, mDryBuffer.get(), numSamplesToRender);
}

void DryWetModule::reset()
{
    if (mDryBuffer != nullptr)
        FloatVectorOperations::clear (mDryBuffer.get(), mBlockSize);
}

void DryWetModule::setState (float dryWetPercent)
{
    mDryWetValue = dryWetPercent / 100.0f;      // convert from %
}
