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
    // Reset LFO phases
    mPhase = 0.0f;
    mAmPhase = 0.0f;

    // Reset smoothed parameters
    mRateSmoothed.reset (mSampleRate, 0.05f);
    mAmplitudeSmoothed.reset (mSampleRate, 0.05f);
    mPhaseOffsetSmoothed.reset (mSampleRate, 0.1f);
}

void LfoModule::process (const float* /*inBuffer*/, float* outBuffer,
                         int numSamplesToRender)
{
    // MARK: Ignoring input buffer

    for (int i = 0; i < numSamplesToRender; ++i)
    {
        const float amAmplitude = 0.3f;
        const float phaseOffset = mPhaseOffsetSmoothed.getNextValue();
        const float modulationPhaseOffset
        = mShouldOffsetModulation ? phaseOffset : 0.0f;

        const float modulationSample = sinf (MathConstants<float>::twoPi * mPhase
                                             + modulationPhaseOffset);
        const float amPhaseOffset = mShouldOffsetModulation ? 0.0f : phaseOffset;
        const float amSample = 1.0f + amAmplitude
        * (sinf (MathConstants<float>::twoPi * mAmPhase + amPhaseOffset) - 1.0f);

        // Record the LFO state in the output buffer
        outBuffer[i] = modulationSample * amSample * mAmplitudeSmoothed.getNextValue();

        // Advance the LFO phase
        const float phaseIncrement = mRateSmoothed.getNextValue() / mSampleRate;
        mPhase += phaseIncrement;

        if (mPhase > 1.0f)
            mPhase -= 1.0f;

        // Advance the extra hamonic phase
        mAmPhase += 1.33f * phaseIncrement;

        if (mAmPhase > 1.0f)
            mAmPhase -= 1.0f;
    }
}

void LfoModule::setState (float rate, float amplitude, float phaseOffsetPercent,
                          bool shouldOffsetModulation)
{
    mRateSmoothed.setTargetValue (rate);
    mAmplitudeSmoothed.setTargetValue (amplitude);
    // Convert % value to radians (100% = pi)
    mPhaseOffsetSmoothed.setTargetValue (MathConstants<float>::pi
                                         * (phaseOffsetPercent / 100.0f));
    mShouldOffsetModulation = shouldOffsetModulation;
}
