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
#include "RBDAudioUtilities.h"

// TODO: DSP module base class

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
    // Map inGain [0, 1] to the new [-24dB, +24dB] range
    float gainMapped = jmap (gain, -24.0f, 24.0f);
    // Convert decibels to gain
    gainMapped = Decibels::decibelsToGain (gainMapped, -24.0f);
    
    for (int i = 0; i < numSamplesToRender; ++i)
        outAudio[i] = inAudio[i] * gainMapped;
    
    // TODO: We're already smoothing audio level in RBDMeter. Do we need to smooth it twice?
    // TODO: This metering depends on the buffer size, sample rate. Look up real metering algorithms
    //  Here we are not really averaging, and we can miss peaks
    //  JUCE AudioBuffer class has a built-in RMS method - use it instead?
    float absSampleValue = fabs (outAudio[0]);
    mLevelSmoothed = mLevelSmoothed - RBD::meterSmoothingCoef * (mLevelSmoothed - absSampleValue);
    //mLevelSmoothed = absSampleValue;
}

float GainModule::getMeterLevel() const
{
    return RBD::remappedMeterLevel (mLevelSmoothed);
}

