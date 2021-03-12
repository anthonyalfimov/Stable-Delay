/*
  ==============================================================================

    RBDLfoModule.h
    Created: 5 Jan 2021 4:58:17pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDDspModule.h"

class LfoModule  : public DspModule
{
public:
    LfoModule();
    ~LfoModule();

    void prepare (double sampleRate, int blockSize) override;
    void process (const float* inBuffer, float* outBuffer,
                  int numSamplesToRender) override;
    void reset() override;

    void setState (float rate, float depthPercent, float phaseOffsetPercent);
    
private:
    float mRateValue = 1.0f;
    float mDepthValue = 0.0f;
    float mPhaseOffset = 0.0f;

    float mPhase = 0.0f;
};
