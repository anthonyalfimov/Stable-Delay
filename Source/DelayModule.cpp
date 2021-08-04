/*
  ==============================================================================

    DelayModule.cpp
    Created: 4 Aug 2021 4:23:23pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "DelayModule.h"

DelayModule::DelayModule (double maxDelayInSeconds)
    : mMaxDelayInSeconds (maxDelayInSeconds)
{

}

//==============================================================================

float DelayModule::read (double delayInSeconds) const
{
    jassert (isPositiveAndNotGreaterThan (delayInSeconds, mMaxDelayInSeconds));

    // Constrain delay time to a valid range
    delayInSeconds = jlimit (0.0, mMaxDelayInSeconds, delayInSeconds);

    const double delayInSamples = delayInSeconds * mSampleRate;
    return getInterpolatedSample (delayInSamples);
}

void DelayModule::writeAndAdvance (float sample)
{
    // Write passed sample to the buffer
    mAudioBuffer[mWritePosition] = sample;

    // Advance the write position
    mWritePosition = (mWritePosition + 1) % mAudioBufferSize;
}

//==============================================================================

void DelayModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    // Add one extra sample just in case
    mAudioBufferSize = static_cast<int> (mMaxDelayInSeconds * mSampleRate + 1);
    mAudioBuffer = std::make_unique<float[]> (mAudioBufferSize);

    reset();
}

void DelayModule::reset()
{
    // Clear the delay buffer
    if (mAudioBuffer != nullptr)
        FloatVectorOperations::clear (mAudioBuffer.get(), mAudioBufferSize);
}

//==============================================================================

void DelayModule::setProcessDelay (double delayInSeconds)
{
    mProcessDelayInSeconds = delayInSeconds;
}

void DelayModule::process (const float* inAudio, float* outAudio,
                           int numSamplesToRender)
{
    for (int i = 0; i < numSamplesToRender; ++i)
    {
        float readSample = read (mProcessDelayInSeconds);
        writeAndAdvance (inAudio[i]);
        outAudio[i] = readSample;
    }
}

//==============================================================================

float DelayModule::getInterpolatedSample (double delayInSamples) const
{
    double readPosition = static_cast<double> (mWritePosition) - delayInSamples;

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
