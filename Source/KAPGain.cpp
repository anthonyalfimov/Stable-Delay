/*
  ==============================================================================

    KAPGain.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KAPGain.h"

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
}

