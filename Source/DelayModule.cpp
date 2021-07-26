/*
  ==============================================================================

    DelayModule.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include "DelayModule.h"
#include "AudioConstants.h"

DelayModule::DelayModule()
{
    // Set the saturation curve
    mSaturator.setState (SaturationCurve::beta);
}

void DelayModule::setState (float time, float feedback, float type,
                            float modRate, float modDepth, float stereoWidth,
                            bool shouldOffsetModulation)
{
    mTypeValue = static_cast<FxType::Index> (type);

    float modAmplitude;

    switch (mTypeValue)
    {
        case FxType::Delay:
            mTimeSmoothed.setTargetValue (time / 1000.0);   // convert from ms to s
            mFeedbackSmoothed.setTargetValue (feedback / 100.0f);    // convert from %
            modRate = 0.05f;        // set fixed slow modulation

            // Set the max modulation amplitude to 0.1 the delay time - as long
            //  as it fits between min and max values for delay time modulation
            //  amlitude
            modAmplitude = (stereoWidth / 100.0f)
                            * jlimit (minDelayTimeAmplitude,
                                      maxDelayTimeAmplitude,
                                      time / 10000.0f);
            break;

        case FxType::Chorus:
            mTimeSmoothed.setTargetValue (chorusCentreTime);
            mFeedbackSmoothed.setTargetValue (0.0f);    // disable feedback
            modAmplitude = chorusTimeAmplitude * modDepth / 100.0f;  // convert from %
            break;

        case FxType::Flanger:
            mTimeSmoothed.setTargetValue (flangerCentreTime);
            mFeedbackSmoothed.setTargetValue (feedback / 100.0f);    // convert from %
            modAmplitude = flangerTimeAmplitude * modDepth / 100.0f; // convert from %
            break;

        default:
            jassertfalse;
            modAmplitude = 0.0f;    // disable modulation
            break;
    }

    const float modOffset = shouldOffsetModulation ? stereoWidth : 0.0f;

    mLfo.setState (modRate, modAmplitude, modOffset);
}

void DelayModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    mLfo.prepare (sampleRate, blockSize);
    mSaturator.prepare (sampleRate, blockSize);

    // Add one extra sample just in case, to account for rounding mistakes, etc.
    mAudioBufferSize = (RBD::maxDelayTimeInSeconds + maxDelayTimeAmplitude)
                        * mSampleRate + 1;
    mAudioBuffer = std::make_unique<float[]> (mAudioBufferSize);
    mModulationBuffer = std::make_unique<float[]> (mBlockSize);
    reset();
}

void DelayModule::reset()
{
    // Clear the delay buffer
    if (mAudioBuffer != nullptr)
        FloatVectorOperations::clear (mAudioBuffer.get(), mAudioBufferSize);

    // Clear the modulation buffer
    if (mModulationBuffer != nullptr)
        FloatVectorOperations::clear (mModulationBuffer.get(), mBlockSize);

    // Reset the LFO
    mLfo.reset();

    // Reset the Saturator
    mSaturator.reset();

    // Reset smoothed parameters
    mTimeSmoothed.reset (mSampleRate, 0.1);
    mFeedbackSmoothed.reset (mSampleRate, 0.05);
}

void DelayModule::process (const float* inAudio, float* outAudio,
                           int numSamplesToRender)
{
    jassert (isPositiveAndNotGreaterThan (numSamplesToRender, mBlockSize));

    // Fill modulation buffer
    mLfo.process (mModulationBuffer.get(), mModulationBuffer.get(), numSamplesToRender);

    // TODO: Consider wrapping delay buffer in an AudioBlock for processing

    for (int i = 0; i < numSamplesToRender; ++i)
    {
    // READ SAMPLE FROM THE DELAY BUFFER
        // MARK: Enforce double precision
        const double delayTimeInSeconds = mTimeSmoothed.getNextValue()
                                          + mModulationBuffer[i];
        
        const double delayTimeInSamples = delayTimeInSeconds * mSampleRate;
        const float readSample = getInterpolatedSample (delayTimeInSamples);

        // We must assume that inAudio and outAudio can point to the same location.
        // Therefore, we must finish reading data from inAudio before writing
        // to outAudio.

    // WRITE SAMPLE TO THE DELAY BUFFER
        float writeSample = inAudio[i]
                            + readSample * mFeedbackSmoothed.getNextValue();

        // Pass the signal through the saturator before writing it to the buffer
        mSaturator.process (&writeSample, &writeSample, 1);
        mAudioBuffer[mWritePosition] = writeSample;

    // WRITE OUTPUT AUDIO
        outAudio[i] = readSample;       // write output audio

    // ADVANCE THE WRITE HEAD
        mWritePosition = (mWritePosition + 1) % mAudioBufferSize;
    }
}

float DelayModule::getInterpolatedSample (double delayTimeInSamples) const
{
    double readPosition = static_cast<double> (mWritePosition) - delayTimeInSamples;
    
    if (readPosition < 0.0)
        readPosition += mAudioBufferSize;
    
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % mAudioBufferSize;
    
    const float readSample0 = mAudioBuffer[readPositionIndex0];
    const float readSample1 = mAudioBuffer[readPositionIndex1];
    
    const double fractionalReadPosition = readPosition - readPositionIndex0;

    // Linear interpolation:
    return readSample0 + fractionalReadPosition * (readSample1 - readSample0);
}

