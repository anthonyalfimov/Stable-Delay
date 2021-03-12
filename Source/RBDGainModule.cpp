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

void GainModule::process (const float* inAudio, float* outAudio,
                          int numSamplesToRender)
{
    for (int i = 0; i < numSamplesToRender; ++i)
        outAudio[i] = inAudio[i] * mGainValue;
}

void GainModule::reset()
{

}

void GainModule::setState (float gainInDecibels)
{
    mGainValue = Decibels::decibelsToGain (gainInDecibels); // convert from decibels
}

