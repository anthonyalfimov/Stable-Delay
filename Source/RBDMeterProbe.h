/*
  ==============================================================================

    RBDMeterProbe.h
    Created: 12 Mar 2021 6:26:27pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDDspModule.h"

class MeterProbe  : public DspModule
{
public:
    MeterProbe();
    ~MeterProbe();

    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

    const std::atomic<float>* getRmsLevel() const { return &mRmsLevel; }

    /** Returns the maximum peak level observed since the last peak level reset.

        @Discussion
        Make sure to call resetPeakLevel() after retrieving a peak level if you
        need real-time peak data. This ensures that the meter will not miss
        peaks that occur between meter repaints.

        If you need to retrieve the highest peak value since plugin initialisation,
        you should not call resetPeakLevel().
    */
    const std::atomic<float>* getPeakLevel() const { return &mPeakLevel; }

    /** Resets the stored maximum peak level to 0 (gain units).

        @Discussion
        If you want getPeakLevel() to return the maximum peak level since last
        retrieval, you need to call resetPeakLevel() after every retrieval.
        Without resetting the peak level, getPeakLevel() returns the highest
        peak since plugin initialisation.
    */
    void resetPeakLevel() { mPeakLevel.store (0.0f); }

    /** Resets the stored RMS level to 0 (gain units).

        @Discussion
        Unlike peak level, RMS doesn't have to be reset after every retrieval
        for real-time measurements. However, you need to call this after
        calling getRmsLevel() if you want the RMS level reading to decay to 0
        when processBlock is not being called (e.g. when plugin is bypassed
        by the host).
    */
    void resetRmsLevel() { mRmsLevel.store (0.0f); }

    void setSuspended (bool isSuspended) { mIsSuspended.store (isSuspended); }

private:
    // Flag that allows to suspend the metering (e.g. when Editor is destroyed)
    std::atomic<bool> mIsSuspended = true;
    
    std::atomic<float> mRmsLevel = 0.0f;
    std::atomic<float> mPeakLevel = 0.0f;

    // RMS window should be larger than the period of the lowest frequency you
    //  expect to be measuring. Larger window will result in slower response
    inline static const float rmsWindowInSeconds = 0.3f;
    int mRmsBufferSize = 0;
    std::unique_ptr<float[]> mRmsBuffer;
    int mWritePosition = 0;

    //==========================================================================
    float calculateRms() const;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterProbe)
};
