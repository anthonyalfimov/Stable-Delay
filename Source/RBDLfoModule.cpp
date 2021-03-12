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
    
}

LfoModule::~LfoModule()
{

}

void LfoModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);
    reset();
}

void LfoModule::process (const float* /*inBuffer*/, float* outBuffer,
                         int numSamplesToRender)
{
    // Ignoring input buffer

    for (int i = 0; i < numSamplesToRender; ++i)
    {
        // Record the LFO state in the output buffer
        outBuffer[i] = mDepthValue * sinf (mPhase * MathConstants<float>::twoPi
                                          + mPhaseOffset);

        // Advance the LFO phase
        mPhase += mRateValue / mSampleRate;

        if (mPhase > 1.0f)
            mPhase -= 1.0f;
    }
}

void LfoModule::reset()
{
    mPhase = 0.0f;
}

void LfoModule::setState (float rate, float depthPercent, float phaseOffsetPercent)
{
    mRateValue = rate;
    mDepthValue = depthPercent / 100.0f;        // convert from %
    mPhaseOffset = MathConstants<float>::pi
                    * (phaseOffsetPercent / 100.0f);    // convert % to radians
}
