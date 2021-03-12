/*
  ==============================================================================

    RBDDelayModule.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDDelayModule.h"
#include "RBDAudioUtilities.h"
#include "RBDAudioConstants.h"

DelayModule::DelayModule()
{
    // Set the saturation curve
    mSaturator.setState (SaturationCurve::beta);
}

DelayModule::~DelayModule()
{
    
}

void DelayModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    mLfo.prepare (sampleRate, blockSize);
    mSaturator.prepare (sampleRate, blockSize);

    mAudioBufferSize = mSampleRate * RBD::maxDelayTimeInSeconds;
    mAudioBuffer = std::make_unique<float[]> (mAudioBufferSize);
    mModulationBuffer = std::make_unique<float[]> (mBlockSize);
    reset();
}

void DelayModule::process (const float* inAudio, float* outAudio,
                           int numSamplesToRender)
{
    jassert (isPositiveAndNotGreaterThan (numSamplesToRender, mBlockSize));

    // Fill modulation buffer
    mLfo.process (mModulationBuffer.get(), mModulationBuffer.get(), mBlockSize);

    // TODO: Consider wrapping delay buffer in an AudioBlock for processing

    // FIXME: FX type switching produces strong chirp
    //  The chirp is due to smoothed delay time change. Slower smoothing
    //  will fix this.

    for (int i = 0; i < numSamplesToRender; ++i)
    {
    // READ SAMPLE FROM THE DELAY BUFFER
        double targetTime = 0.0;

        switch (mTypeValue)
        {
            case FxType::Delay:
                targetTime = mTimeValue;
                break;

            case FxType::Chorus:
                targetTime = jmap (mModulationBuffer[i],
                                   -1.0f, 1.0f, 0.001f, 0.010f);
                break;

            case FxType::Flanger:
                targetTime = jmap (mModulationBuffer[i],
                                   -1.0f, 1.0f, 0.0005f, 0.0050f);
                break;

            default:
                jassertfalse;
                break;
        }

        mTimeSmoothed = mTimeSmoothed + RBD::smoothingCoefficient
                                        * (targetTime - mTimeSmoothed);

        const double delayTimeInSamples = mTimeSmoothed * mSampleRate;
        const float readSample = getInterpolatedSample (delayTimeInSamples);

    // WRITE SAMPLE TO THE DELAY BUFFER
        float writeSample = inAudio[i] + mFeedbackSample * mFeedbackValue;

        // Pass the signal through the saturator before writing it to the buffer
        mSaturator.process (&writeSample, &writeSample, 1);
        mAudioBuffer[mWritePosition] = writeSample;

    // WRITE OUTPUT AUDIO
        mFeedbackSample = readSample;   // update feedback sample
        outAudio[i] = readSample;       // write output audio

    // ADVANCE THE WRITE HEAD
        mWritePosition = (mWritePosition + 1) % mAudioBufferSize;
    }
}

void DelayModule::reset()
{
    // Clear the feedback sample
    mFeedbackSample = 0.0f;

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
}

void DelayModule::setState (float time, float feedback, float type,
                            float modRate, float modDepth, float modOffset)
{
    mTimeValue = time / 1000.0f;        // convert from ms to s
    mTypeValue = static_cast<FxType::Index> (type);

    // Convert from % (disable feedback if FX type is chorus)
    mFeedbackValue = (mTypeValue == FxType::Chorus) ? 0.0f : feedback / 100.0f;

    mLfo.setState (modRate, modDepth, modOffset);
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

