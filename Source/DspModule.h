/*
  ==============================================================================

    DspModule.h
    Created: 12 Mar 2021 6:08:51pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

// TODO: DSP modules per channel -> DSP modules processing vectorised channels
//  The downside of isolating each channel to its own set of DSP modules is that
//  the channels cannot interact without creating dedicated interfaces.

class DspModule
{
public:
    DspModule() = default;
    virtual ~DspModule() = default;

//==============================================================================
    virtual void prepare (double sampleRate, int blockSize);
    virtual void reset() = 0;
    
    // TODO: Switch from raw buffer pointers to JUCE AudioBlock or similar
    //  We can pass JUCE AudioBlock objects wrapping the audio buffer,
    //  references to the audio buffer itself (bad idea?),
    //  or wrap whatever we're passing into ProcessContext-style object

    virtual void process (const float* inAudio, float* outAudio,
                          int numSamplesToRender) = 0;

protected:
    double mSampleRate = 44100.0;
    int mBlockSize = 0;
};
