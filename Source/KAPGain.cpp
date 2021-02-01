/*
  ==============================================================================

    KAPGain.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KAPGain.h"
#include "KAPAudioHelpers.h"

// TODO: DSP module base class

KAPGain::KAPGain()
{
    
}

KAPGain::~KAPGain()
{
    
}

void KAPGain::process (const float* inAudio,
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
    
    // TODO: We're already smoothing audio level in KAPMeter. Do we need to smooth it twice?
    // TODO: This metering depends on the buffer size, sample rate. Implement proper metering
    //  Here we are not really averaging, and we can miss peaks. Look up real metering algorithms
    float absSampleValue = fabs (outAudio[0]);
    mLevelSmoothed = mLevelSmoothed - KAP::meterSmoothingCoef * (mLevelSmoothed - absSampleValue);
    //mLevelSmoothed = absSampleValue;
}

float KAPGain::getMeterLevel() const
{
    return KAP::remappedMeterLevel (mLevelSmoothed);
}

