/*
  ==============================================================================

    DspModule.cpp
    Created: 24 Apr 2021 12:05:11am
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DspModule.h"

void DspModule::prepare (double sampleRate, int blockSize)
{
    jassert (sampleRate > 0.0);
    jassert (blockSize > 0);

    mSampleRate = sampleRate;
    mBlockSize = blockSize;
}
