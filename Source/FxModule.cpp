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

void FxModule::setState (float driveInDecibels, bool applyBoost,
                         float time, float feedback, float type,
                         float modRate, float modDepth, float stereoWidth,
                         bool shouldOffsetModulation)
{
    // Set delay input drive parameters
    
    mDriveSmoothed.setTargetValue (driveInDecibels);
    const float feedbackCompensationFactor = Decibels::decibelsToGain (driveInDecibels);

    // Set delay and modulation parameters
    mTypeValue = static_cast<FxType::Index> (type);

    float modAmplitude = 0.0f;

    switch (mTypeValue)
    {
        case FxType::Delay:
        {
            mTimeSmoothed.setTargetValue (time / 1000.0);   // convert from ms to s

            // Compensate feedback reduction due to post-saturation gain decrease
            feedback = feedback * feedbackCompensationFactor / 100.0f; // convert from %
            mFeedbackSmoothed.setTargetValue (feedback);

            // TODO: Consider using PiecewiseNormalisableRange to map delay modulation

            modRate = delayModRate; // set fixed slow modulation
            // Set the max modulation amplitude to 0.1 the delay time - as long
            //  as it fits between min and max values for delay time modulation
            //  amlitude
            modAmplitude = (stereoWidth / 100.0f) * jlimit (minDelayTimeAmplitude,
                                                            maxDelayTimeAmplitude,
                                                            time / 10000.0f);
            break;
        }

        case FxType::Chorus:
        {
            mTimeSmoothed.setTargetValue (chorusCentreTime);

            mFeedbackSmoothed.setTargetValue (0.0f);    // disable feedback

            modAmplitude = chorusTimeAmplitude * modDepth / 100.0f;  // convert from %
            break;
        }

        case FxType::Flanger:
        {
            mTimeSmoothed.setTargetValue (flangerCentreTime);

            // Compensate feedback reduction due to post-saturation gain decrease
            feedback = feedback * feedbackCompensationFactor / 100.0f; // convert from %
            mFeedbackSmoothed.setTargetValue (feedback);

            modAmplitude = flangerTimeAmplitude * modDepth / 100.0f; // convert from %
            break;
        }

        default:
        {
            jassertfalse;
            break;
        }
    }

    const float modOffset = shouldOffsetModulation ? stereoWidth : 0.0f;
    mLfo.setState (modRate, modAmplitude, modOffset);
}

void FxModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    // Prepare contained modules
    mSaturator.prepare (sampleRate, blockSize);
    mDelay.prepare (sampleRate, blockSize);
    mLfo.prepare (sampleRate, blockSize);

    // Allocate modulation buffer
    mModulationBuffer = std::make_unique<float[]> (mBlockSize);
    
    reset();
}

void FxModule::reset()
{
    // Clear the modulation buffer
    if (mModulationBuffer != nullptr)
        FloatVectorOperations::clear (mModulationBuffer.get(), mBlockSize);

    // Reset contained modules
    mSaturator.reset();
    mDelay.reset();
    mLfo.reset();

    // Reset smoothed parameters
    mDriveSmoothed.reset (mSampleRate, 0.05f);
    mTimeSmoothed.reset (mSampleRate, 0.1);
    mFeedbackSmoothed.reset (mSampleRate, 0.05f);
}

void FxModule::process (const float* inAudio, float* outAudio,
                        int numSamplesToRender)
{
    // FILL MODULATION BUFFER
    mLfo.process (mModulationBuffer.get(), mModulationBuffer.get(), numSamplesToRender);

    for (int i = 0; i < numSamplesToRender; ++i)
    {
    // READ SAMPLE FROM THE DELAY BUFFER
        // MARK: Enforce double precision
        const double delayTimeInSeconds = mTimeSmoothed.getNextValue()
                                          + mModulationBuffer[i];
        const float readSample = mDelay.read (delayTimeInSeconds);

        // We must assume that inAudio and outAudio can point to the same location.
        // Therefore, we must finish reading data from inAudio before writing
        // to outAudio.

    // WRITE SAMPLE TO THE DELAY BUFFER AND ADVANCE THE WRITE HEAD
        const float drive = mDriveSmoothed.getNextValue();
        const float halfDrive = drive / 2.0f;
        const float inSample = inAudio[i];
        float writeSample = inSample * Decibels::decibelsToGain (drive)
                          + readSample * mFeedbackSmoothed.getNextValue();

        // Pass the signal through the saturator before writing it to the buffer
        mSaturator.process (&writeSample, &writeSample, 1);
        // Apply post-saturator gain
        const float inPeakLevel = Decibels::gainToDecibels (std::abs (inSample));
        const float smoothingFactor = 4.0f;
        const float postGainInDecibels
        = 0.5f * halfDrive * (std::tanh ((inPeakLevel + drive) / smoothingFactor) - 3.0f);
        writeSample = writeSample * Decibels::decibelsToGain (postGainInDecibels);

        mDelay.writeAndAdvance (writeSample);

    // WRITE OUTPUT AUDIO
        outAudio[i] = readSample;       // write output audio
    }
}

