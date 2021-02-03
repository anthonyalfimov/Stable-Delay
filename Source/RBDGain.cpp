/*
  ==============================================================================

    RBDGain.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDGain.h"
#include "RBDAudioHelpers.h"

// TODO: DSP module base class

RBDGain::RBDGain()
{
    
}

RBDGain::~RBDGain()
{
    
}

void RBDGain::process (const float* inAudio,
                       float inGain,
                       float* outAudio,
                       int inNumSamplesToRender)
{
    // Map inGain [0, 1] to the new [-24dB, +24dB] range
    float gainMapped = jmap (inGain, 0.0f, 1.0f, -24.0f, 24.0f);
    // Convert decibels to gain
    gainMapped = Decibels::decibelsToGain (gainMapped, -24.0f);
    
    for (int i = 0; i < inNumSamplesToRender; ++i)
        outAudio[i] = inAudio[i] * gainMapped;
    
    // TODO: We're already smoothing audio level in RBDMeter. Do we need to smooth it twice?
    // TODO: This metering depends on the buffer size, sample rate. Look up real metering algorithms
    //  Here we are not really averaging, and we can miss peaks
    float absSampleValue = fabs (outAudio[0]);
    mLevelSmoothed = mLevelSmoothed - RBD::meterSmoothingCoef * (mLevelSmoothed - absSampleValue);
    //mLevelSmoothed = absSampleValue;
}

float RBDGain::getMeterLevel() const
{
    return RBD::remappedMeterLevel (mLevelSmoothed);
}

