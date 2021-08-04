/*
  ==============================================================================

    FxModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspModule.h"
#include "LfoModule.h"
#include "SaturationModule.h"
#include "Parameters.h"

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

class FxModule  : public DspModule
{
public:
    FxModule();

//==============================================================================
    void setState (float time, float feedback, float type,
                   float modRate, float modDepth, float stereoWidth,
                   bool shouldOffsetModulation);
    
//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
//==============================================================================
    // Parameters
    // MARK: Precision: time smoothing
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> mTimeSmoothed;
    SmoothedValue<float> mFeedbackSmoothed;
    FxType::Index mTypeValue = FxType::Delay;

//==============================================================================
    // FX type constants
    inline static constexpr double chorusCentreTime = 0.006;
    inline static constexpr float chorusTimeAmplitude = 0.005f;
    inline static constexpr double flangerCentreTime = 0.0027;
    inline static constexpr float flangerTimeAmplitude = 0.0023f;
    inline static constexpr float maxDelayTimeAmplitude = 0.01f;
    inline static constexpr float minDelayTimeAmplitude = 0.0005f;

//==============================================================================
    // Delay

    // TODO: Consider a better data structure for holding the delay buffer
    //  JUCE AudioBuffer template should be a good choice. It is designed to hold
    //  multiple channels, while our current design handles multiple channesl by creating
    //  the appropriate number of DSP modules. Withing the DSP module itself, there's
    //  always just one channel. Switching to JUCE AudioBuffer makes sense once we also
    //  switch to having a single instance of each DSP module handling vectorised channels

    std::unique_ptr<float[]> mAudioBuffer;
    int mAudioBufferSize = 0;
    
    int mWritePosition = 0;

    float getInterpolatedSample (double delayTimeInSamples) const;

//==============================================================================
    // LFO
    LfoModule mLfo;
    // MARK: Precision: modulation
    std::unique_ptr<float[]> mModulationBuffer;

//==============================================================================
    // Saturation
    SaturationModule mSaturator;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxModule)
};

