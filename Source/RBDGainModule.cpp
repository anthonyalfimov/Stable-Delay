/*
  ==============================================================================

    RBDGainModule.cpp
    Created: 5 Jan 2021 4:57:54pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDGainModule.h"

GainModule::GainModule()
{
    
}

GainModule::~GainModule()
{
    
}

void GainModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);
    reset();
}

void GainModule::reset()
{
    // Reset smoothed parameters
    mGainSmoothed.reset (mSampleRate, 0.05f);
}

void GainModule::process (const float* inAudio, float* outAudio,
                          int numSamplesToRender)
{
    for (int i = 0; i < numSamplesToRender; ++i)
        outAudio[i] = inAudio[i] * mGainSmoothed.getNextValue();
}

void GainModule::setState (float gainInDecibels)
{
    // Convert from decibels
    mGainSmoothed.setTargetValue (Decibels::decibelsToGain (gainInDecibels));
}

