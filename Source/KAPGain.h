/*
  ==============================================================================

    KAPGain.h
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

namespace KAP
{
    class Gain
    {
    public:
        Gain();
        ~Gain();
        
        // TODO: using raw pointers for audio buffers here.. is it safe? is there a better way?
        void process (const float* inAudioBuffer,
                      float inGain,
                      float* outAudioBuffer,
                      int inNumSamplesToRender);
        
    private:
    };
}
