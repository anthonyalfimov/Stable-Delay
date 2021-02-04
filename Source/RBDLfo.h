/*
  ==============================================================================

    RBDLfo.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDAudioConstants.h"

class RBDLfo
{
public:
    RBDLfo();
    ~RBDLfo();
    
    void setSampleRate (double inSampleRate);
    void reset();
    void process(float inRate,
                 float inDepth,
                 int inNumSamplesToRender);
    
    const float* getBuffer() const;
    
private:
    double mSampleRate = -1.0;
    float mPhase = 0.0f;
    float mBuffer[RBD::bufferSize];     // TODO: dynamically allocate the buffer
    
    // TODO: JUCE DSP modules don't use the Leak Detector. Should I?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDLfo)
};
