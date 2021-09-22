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
    double delayInSamples = delayInSeconds * mSampleRate;
    const double maxDelayInSamples = mAudioBufferSize;

    jassert (isPositiveAndNotGreaterThan (delayInSamples, maxDelayInSamples));
    // This delay design enforces reading before writing, so it doesn't support
    //  0-sample delay time
    delayInSamples = jlimit (1.0, maxDelayInSamples, delayInSamples);

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

    // At max delay time the read position coincides with the write position.
    //  This is ok, since this delay design enforces reading before writing.
    mAudioBufferSize = static_cast<int> (mMaxDelayInSeconds * mSampleRate);
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

    /* Temporarily disable intepolarion: */
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % mAudioBufferSize;

    const float readSample0 = mAudioBuffer[readPositionIndex0];
    const float readSample1 = mAudioBuffer[readPositionIndex1];

    const double fractionalReadPosition = readPosition - readPositionIndex0;

    // Linear interpolation:
    return readSample0 + fractionalReadPosition * (readSample1 - readSample0);
    
    /*
    const int readPositionIndex = static_cast<int> (readPosition);
    return mAudioBuffer[readPositionIndex];
    */
}
