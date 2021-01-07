/*
  ==============================================================================

    KAPDelay.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPAudioHelpers.h"

class KAPDelay
{
public:
    KAPDelay();
    ~KAPDelay();
    
    void setSampleRate (double inSampleRate);
    void reset();
    void process (const float* inAudio,
                  float inTime,
                  float inFeedback,
                  float inWetDry,
                  float* outAudio,
                  int inNumSamplesToRender);
    
private:
    float getInterpolatedSample (double inDelayTimeInSamples) const;
    
    double mSampleRate;
    float mFeedbackSampe;
    float mBuffer[KAP::bufferSize];   // TODO: make buffer dynamic
    
    int mDelayIndex;
};

