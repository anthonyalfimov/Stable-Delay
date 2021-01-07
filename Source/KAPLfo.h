/*
  ==============================================================================

    KAPLfo.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPAudioHelpers.h"

class KAPLfo
{
public:
    KAPLfo();
    ~KAPLfo();
    
    void setSampleRate (double inSampleRate);
    void reset();
    void process(float inRate,
                 float inDepth,
                 int inNumSamplesToRender);
    
    const float* getBuffer() const;
    
private:
    double mSampleRate;
    float mPhase;
    float mBuffer[KAP::bufferSize];     // TODO: dynamically allocate the buffer
};
