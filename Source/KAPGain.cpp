/*
  ==============================================================================

    KAPGain.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KAPGain.h"

namespace KAP
{
    // TODO: DSP module base class

    Gain::Gain()
    {
        
    }
    
    Gain::~Gain()
    {
        
    }
    
    void Gain::process (const float* inAudioBuffer,
                        float inGain,
                        float* outAudioBuffer,
                        int inNumSamplesToRender)
    {
        // Map inGain [0, 1] to the new [-24dB, +24dB] range
        float gainMapped = jmap (inGain, 0.0f, 1.0f, -24.0f, 24.0f);
        // Convert decibels to gain
        gainMapped = Decibels::decibelsToGain (gainMapped, -24.0f);
        
        for (int sample = 0; sample < inNumSamplesToRender; ++sample)
            outAudioBuffer[sample] = inAudioBuffer[sample] * gainMapped;
    }
}
