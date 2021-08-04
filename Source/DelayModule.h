/*
  ==============================================================================

    DelayModule.h
    Created: 4 Aug 2021 4:23:23pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"

// TODO: Choose where to use double precision for delay time
//  Extra precision is beneficial when values of significantly different orders
//  of magnitude are combined. Without increasing precision, some detail might
//  be lost due to a limited number of significant digits.
//
//  Roughly: float  -  7 significant decimal digits,
//           double - 16 significant decimal digits.
//
//  Should double precision be used for:
//
//  1) mTimeSmoothed and mModulationBuffer
//      - control inputs; delay time in seconds is controlled by a float
//      parameter, so double precision would only affect smoothing resolution;
//      => use single precision by default;
//      => test whether time parameter smoothing and modulation benefit from
//      extra resolution of double precision;
//
//  2) delay time in seconds
//      - sum of the time parameter value and the modulation; these can have
//      significantly different orders of magnitude;
//      => use double precision;
//
//  3) delay time in samples
//      - high sample rate and/or delay time can result in almost no significant
//      digits left for the fractional part when using type float;
//      - fractional part of the delay time in samples is especially important
//      when using good intersample interpolation;
//      => use double precision.

class DelayModule  : public DspModule
{
public:
    explicit DelayModule (double maxDelayInSeconds);

//==============================================================================
    /**
        Returns a sample delayed from the current write position by the passed
        delay time value.
    */
    float read (double delayInSeconds) const;

    /**
        Writes the passed sample into the delay buffer at the current write
        position and advances it by one.
    */
    void writeAndAdvance (float sample);

//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;

//==============================================================================
    /**
        Set the delay time that will be used by the `process` method.
    */
    void setProcessDelay (double delayInSeconds);

    /**
        Delay passed audio buffer by an amount of time specified by calling
        `setProcessDelay` method.
    */
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
    // TODO: Consider a better data structure for holding the delay buffer
    //  JUCE AudioBuffer template should be a good choice. It is designed to hold
    //  multiple channels, while our current design handles multiple channesl
    //  by creating the appropriate number of DSP modules. Withing the DSP module
    //  itself, there's always just one channel. Switching to JUCE AudioBuffer
    //  makes sense once we also switch to having a single instance of each DSP
    //  module handling vectorised channels
    std::unique_ptr<float[]> mAudioBuffer;

    const double mMaxDelayInSeconds = 0.0;
    int mAudioBufferSize = 0;
    int mWritePosition = 0;

//==============================================================================
    double mProcessDelayInSeconds = 0.0;

//==============================================================================
    float getInterpolatedSample (double delayInSamples) const;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayModule)
};
