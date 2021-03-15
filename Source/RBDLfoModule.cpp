/*
  ==============================================================================

    RBDLfoModule.cpp
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

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

void LfoModule::reset()
{
    // Reset LFO phase
    mPhase = 0.0f;

    // Reset smoothed parameters
    mRateSmoothed.reset (mSampleRate, 0.05f);
    mAmplitudeSmoothed.reset (mSampleRate, 0.05f);
    mPhaseOffsetSmoothed.reset (mSampleRate, 0.05f);
}

void LfoModule::process (const float* /*inBuffer*/, float* outBuffer,
                         int numSamplesToRender)
{
    // MARK: Ignoring input buffer

    for (int i = 0; i < numSamplesToRender; ++i)
    {
        // Record the LFO state in the output buffer
        outBuffer[i] = mAmplitudeSmoothed.getNextValue()
                        * sinf (MathConstants<float>::twoPi * mPhase
                                + mPhaseOffsetSmoothed.getNextValue());

        // Advance the LFO phase
        mPhase += mRateSmoothed.getNextValue() / mSampleRate;

        if (mPhase > 1.0f)
            mPhase -= 1.0f;
    }
}

void LfoModule::setState (float rate, float amplitude, float phaseOffsetPercent)
{
    mRateSmoothed.setTargetValue (rate);
    mAmplitudeSmoothed.setTargetValue (amplitude);
    // Convert % value to radians (100% = pi)
    mPhaseOffsetSmoothed.setTargetValue (MathConstants<float>::pi
                                         * (phaseOffsetPercent / 100.0f));
}
