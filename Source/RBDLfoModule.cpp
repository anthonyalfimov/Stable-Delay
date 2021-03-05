/*
  ==============================================================================

    RBDLfoModule.cpp
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDLfoModule.h"

LfoModule::LfoModule()
{
    reset();
}

LfoModule::~LfoModule()
{
    
}

void LfoModule::setSampleRateAndBlockSize (double sampleRate, int samplesPerBlock)
{
    mSampleRate = sampleRate;
    mBufferSize = samplesPerBlock;
    mBuffer = std::make_unique<float[]> (mBufferSize);
    reset();
}

void LfoModule::reset()
{
    // Reset the phase
    mPhase = 0.0f;
    // Clear the buffer
    if (mBuffer != nullptr)
        zeromem (mBuffer.get(), sizeof (float) * mBufferSize);
}

void LfoModule::process (float rate, float depth, int numSamplesToRender)
{
    const float depthMapped = depth / 100.0f;   // convert from %
    for (int i = 0; i < numSamplesToRender; ++i)
    {
        // Record the LFO state in the buffer
        const float lfoPosition = depthMapped
                                    * sinf (mPhase * MathConstants<float>::twoPi);
        mBuffer[i] = lfoPosition;
        
        // Advance the LFO phase
        mPhase += rate / mSampleRate;
        
        if (mPhase > 1.0f)
            mPhase -= 1.0f;
    }
}

const float* LfoModule::getBuffer() const
{
    return mBuffer.get();
}
