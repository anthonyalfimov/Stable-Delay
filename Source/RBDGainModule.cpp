/*
  ==============================================================================

    RBDGainModule.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDGainModule.h"
#include "RBDAudioConstants.h"

GainModule::GainModule()
{
    
}

GainModule::~GainModule()
{
    
}

void GainModule::process (const float* inAudio,
                          float gain,
                          float* outAudio,
                          int numSamplesToRender)
{
    // Convert decibels to gain
    const float gainMapped = Decibels::decibelsToGain (gain);
    
    for (int i = 0; i < numSamplesToRender; ++i)
        outAudio[i] = inAudio[i] * gainMapped;
    
    // TODO: We're already smoothing audio level in RBDMeter. Do we need to smooth it twice?
    // TODO: This metering depends on the buffer size, sample rate. Look up real metering algorithms
    //  Here we are not really averaging, and we can miss peaks
    //  JUCE AudioBuffer class has a built-in RMS method - use it instead?
    float absSampleValue = fabs (outAudio[0]);
    float levelSmoothed = mMeterLevel.load();
    levelSmoothed = levelSmoothed - RBD::meterSmoothingCoef * (levelSmoothed - absSampleValue);
    mMeterLevel.store (levelSmoothed);
}

const std::atomic<float>* GainModule::getMeterLevel() const
{
    return &mMeterLevel;
}

