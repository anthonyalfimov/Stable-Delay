/*
  ==============================================================================

    MeterProbe.cpp
    Created: 12 Mar 2021 6:26:27pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "MeterProbe.h"

void MeterProbe::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);
    mRmsBufferSize = rmsWindowInSeconds * sampleRate;
    mRmsBuffer = std::make_unique<float[]> (mRmsBufferSize);

    reset();
}

void MeterProbe::reset()
{
    // Clear the RMS buffer
    if (mRmsBuffer != nullptr)
        FloatVectorOperations::clear (mRmsBuffer.get(), mRmsBufferSize);

    mWritePosition = 0;

    // Reset level readings
    mRmsLevel.store (0.0f);
    mPeakLevel.store (0.0f);
}

void MeterProbe::process (const float* inAudio, float* /*outAudio*/,
                          int numSamplesToRender)
{
    if (mIsSuspended)
        return;

    // TODO: Consider replacing with FloatVectorOperations
    //  The same can be achieved with FloatVectorOperations
    //  findMinAndMax and copy. Copy would have to check if there's enough
    //  space in the buffer. If not, the copy would need to be split into
    //  two operations: till the end of the buffer, and from the beginning
    //  of the buffer.

    /* JUCE example of using FloatVectorOperations::findMinAndMax():

        auto r = FloatVectorOperations::findMinAndMax (buffer, numSamples);
        return jmax (r.getStart(), -r.getStart(), r.getEnd(), -r.getEnd());
    */

    float maxPeakLevel = mPeakLevel.load();

    for (int i = 0; i < numSamplesToRender; ++i)
    {
        const float sample = inAudio[i];

        // Update peak level
        const float currentPeakLevel = std::abs (sample);

        if (currentPeakLevel > maxPeakLevel)
            maxPeakLevel = currentPeakLevel;

        // Fill RMS buffer with sample value squared
        mRmsBuffer[mWritePosition] = sample * sample;
        mWritePosition = (mWritePosition + 1) % mRmsBufferSize;
    }

    mPeakLevel.store (maxPeakLevel);    // write peak level

    // TODO: We don't need to calculate the RMS level every block
    //  The meter GUI is updated at a slower rate than block processing (~15Hz).
    //  We should determine the maximum acceptable latency in RMS value update.
    //  The RMS level should be updated with a period equal to that latency.
    //  This can be done by running a counter and skipping a number of blocks
    //  before recalculating the RMS.

    mRmsLevel.store (calculateRms());   // write RMS level
}

// TODO: Consider using running sum to avoid summing the buffer every calculation
//  Note that using the running sum could result in accumulated float math error.
//  This can be avoided by using an int buffer, or by periodically recalculating
//  the sum from the buffer.
//  We don't need to calculate RMS in real time or even every block, it can be
//  updated at ~15Hz (meter refresh rate). The running sum can be reset at
//  1 Hz.

float MeterProbe::calculateRms() const
{
    double sum = 0.0;

    for (int i = 0; i < mRmsBufferSize; ++i)
        sum += mRmsBuffer[i];

    return std::sqrt (sum / mRmsBufferSize);
}
