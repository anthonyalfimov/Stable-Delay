/*
  ==============================================================================

    FxModule.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include "FxModule.h"
#include "AudioConstants.h"

FxModule::FxModule()
{
    // Set the saturation curve
    mSaturator.setState (SaturationCurve::beta);
}

void FxModule::setState (float driveInDecibels, float clipRise, float clipFall)
{
    // Set delay input drive parameters
    mDriveSmoothed.setTargetValue (driveInDecibels);

    // Set dynamic threshold detector rise and fall time constants
    mDetector.setState (clipRise, clipFall);
}

void FxModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    // Prepare contained modules
    mDetector.prepare (sampleRate, blockSize);
    mSaturator.prepare (sampleRate, blockSize);

    reset();
}

void FxModule::reset()
{
    // Reset contained modules
    mDetector.reset();
    mSaturator.reset();

    // Reset smoothed parameters
    mDriveSmoothed.reset (mSampleRate, 0.0);
}

void FxModule::process (const float* inAudio, float* outAudio,
                        int numSamplesToRender)
{
    for (int i = 0; i < numSamplesToRender; ++i)
    {
        const float inSample = inAudio[i];

        const float gain = mDetector.processSample (std::abs (inSample));
        const float levelInDb = Decibels::gainToDecibels (gain);
        
        const float thresholdInDb = jlimit (minThreshold, maxThreshold,
                                            levelInDb + clippingThreshold);

        // Apply pre-saturator gain
        const float preBoostInDb = mDriveSmoothed.getNextValue();
        const float postCutInDb = preBoostInDb * postCutFactor;

        float outSample = inSample * Decibels::decibelsToGain (preBoostInDb);
        
        // Apply threshold-move boost
        outSample *= Decibels::decibelsToGain (-thresholdInDb);

        // Pass the signal through the saturator before writing it to the buffer
        mSaturator.process (&outSample, &outSample, 1);

        // Apply threshold-move cut
        outSample *= Decibels::decibelsToGain (thresholdInDb);

        // Apply post-saturator gain
        outSample *= Decibels::decibelsToGain (-postCutInDb);

        // WRITE OUTPUT AUDIO
        outAudio[i] = outSample;
    }
}

