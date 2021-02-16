/*
  ==============================================================================

    RBDLfoModule.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDAudioConstants.h"

class LfoModule
{
public:
    LfoModule();
    ~LfoModule();
    
    void setSampleRateAndBlockSize (double sampleRate, int samplesPerBlock);
    void reset();
    void process(float rate,
                 float depth,
                 int numSamplesToRender);
    
    const float* getBuffer() const;
    
private:
    double mSampleRate = -1.0;
    float mPhase = 0.0f;
    
    std::unique_ptr<float[]> mBuffer;
    int mBufferSize = 0;
    
    // TODO: JUCE DSP modules don't use the Leak Detector. Should I?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDLfo)
};
