/*
  ==============================================================================

    KAPGain.h
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

class KAPGain
{
public:
    KAPGain();
    ~KAPGain();
    
    // TODO: using raw pointers for audio buffers here.. is it safe? is there a better way?
    void process (const float* inAudio,
                  float inGain,
                  float* outAudio,
                  int inNumSamplesToRender);
    
    float getMeterLevel() const;
    
private:
    // TODO: We're already smoothing audio level in KAPMeter. Do we need to smooth it here too?
    float mLevelSmoothed = 0.0f;
    
    // TODO: JUCE DSP modules don't use the Leak Detector. Should I?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPGain)
};

