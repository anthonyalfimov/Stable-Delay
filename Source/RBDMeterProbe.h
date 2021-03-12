/*
  ==============================================================================

    RBDMeterProbe.h
    Created: 12 Mar 2021 6:26:27pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MeterProbe
{
public:
    MeterProbe();
    ~MeterProbe();

    void process (const float* inAudio, int numSamplesToRender);

    const std::atomic<float>* getMeterLevel() const;

private:
    std::atomic<float> mMeterLevel { 0.0f };
};
