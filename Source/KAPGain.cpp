/*
  ==============================================================================

    KAPGain.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPGain.h"

namespace KAP
{

    Gain::Gain()
    {
        
    }
    
    Gain::~Gain()
    {
        
    }
    
    void Gain::process (float* inAudioBuffer,
                        float inGain,
                        float* outAudioBuffer,
                        int inNumSamplesToRender)
    {
        for (int sample = 0; sample < inNumSamplesToRender; ++sample)
            outAudioBuffer[sample] = inAudioBuffer[sample] * inGain;
    }
}
