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
#include "GainModule.h"
#include "SlewFilterModule.h"
#include "SaturationModule.h"
#include "DelayModule.h"
#include "LfoModule.h"
#include "Parameters.h"

class FxModule  : public DspModule
{
public:
    FxModule();

//==============================================================================
    void setState (float driveInDecibels,
                   float time, float feedback, float type,
                   float modRate, float modDepth, float stereoWidth,
                   bool shouldOffsetModulation,
                   bool dynamicClipping, float clipRise, float clipFall,
                   float clipThresholdDelta, float clipMinThreshold,
                   DClip::DetectorMode detectorMode, bool shouldOutputDetector,
                   float postCutFactor);
    
//==============================================================================
    void prepare (double sampleRate, int blockSize) override;
    void reset() override;
    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;

private:
//==============================================================================
    // Parameters
    FxType::Index mTypeValue = FxType::Delay;
    // MARK: Precision: time smoothing
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> mTimeSmoothed;
    SmoothedValue<float> mFeedbackSmoothed;

//==============================================================================
    // FX type constants
    inline static constexpr double chorusCentreTime = 0.006;
    inline static constexpr float chorusTimeAmplitude = 0.005f;
    inline static constexpr double flangerCentreTime = 0.0027;
    inline static constexpr float flangerTimeAmplitude = 0.0023f;
    inline static constexpr float maxDelayTimeAmplitude = 0.01f;
    inline static constexpr float minDelayTimeAmplitude = 0.0005f;

    inline static constexpr float delayModRate = 0.05f;

//==============================================================================
    // Drive
    SmoothedValue<float> mDriveSmoothed;
    SlewFilterModule mDetector;
    SaturationModule mSaturator;

    bool mUseDynamicClipping = true;
    float mClippingThreshold = 8.0f;
    float mMinThreshold = -36.0f;
    DClip::DetectorMode mDetectorMode = DClip::Gain;
    bool mShouldOutputDetector = false;
    float mPostCutFactor = 0.5f;
    
    inline static constexpr float detectorRiseTime = 0.2f /*ms*/;
    inline static constexpr float detectorFallTime = 1200.0f /*ms*/;

//==============================================================================
    // Delay
    DelayModule mDelay { RBD::maxDelayTimeInSeconds + maxDelayTimeAmplitude };

//==============================================================================
    // Modulation
    // MARK: Precision: modulation
    LfoModule mLfo;
    std::unique_ptr<float[]> mModulationBuffer;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxModule)
};

