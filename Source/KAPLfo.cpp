/*
  ==============================================================================

    KAPLfo.cpp
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KAPLfo.h"

KAPLfo::KAPLfo() : mSampleRate (-1.0), mPhase (0.0f)
{
    reset();
}

KAPLfo::~KAPLfo()
{
    
}

void KAPLfo::setSampleRate (double inSampleRate)
{
    mSampleRate = inSampleRate;
}

void KAPLfo::reset()
{
    // Reset the phase
    mPhase = 0.0f;
    // Clear the buffer
    zeromem (mBuffer, sizeof (float) * KAP::bufferSize);
}

void KAPLfo::process(float inRate,
                     float inDepth,
                     int inNumSamplesToRender)
{
    const float rate = jmap(inRate, 0.0f, 1.0f, 0.01f, 10.0f);
    
    for (int i = 0; i < inNumSamplesToRender; ++i)
    {
        // Record the LFO state in the buffer
        const float lfoPosition = inDepth * sinf(mPhase * MathConstants<float>::twoPi);
        mBuffer[i] = lfoPosition;
        
        // Advance the LFO phase
        mPhase += rate / mSampleRate;
        
        if (mPhase > 1.0f)
            mPhase -= 1.0f;
    }
}

const float* KAPLfo::getBuffer() const
{
    return mBuffer;
}
