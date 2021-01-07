/*
  ==============================================================================

    KAPDelay.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KAPDelay.h"

KAPDelay::KAPDelay() : mSampleRate(-1.0), mFeedbackSampe(0.0f), mDelayIndex(0)
{
    reset();
}

KAPDelay::~KAPDelay()
{
    
}

void KAPDelay::setSampleRate (double inSampleRate)
{
    mSampleRate = inSampleRate;
}

void KAPDelay::reset()
{
    // Clear the delay buffer
    zeromem (mBuffer, sizeof (float) * KAP::bufferSize);
}

void KAPDelay::process (const float* inAudio,
                        float inTime,
                        float inFeedback,
                        float inWetDry,
                        float* outAudio,
                        int inNumSamplesToRender)
{
    const float wet = inWetDry;
    const float dry = 1.0f - wet;
    const float feedbackMapped = jmap (inFeedback, 0.0f, 1.0f, 0.0f, 0.95f);
    
    for (int i = 0; i < inNumSamplesToRender; ++i)
    {
        const double delayTimeInSamples = inTime * mSampleRate; // inside the loop for dynamic delay time
        const float sample = getInterpolatedSample (delayTimeInSamples);
        
        // Write to the buffer: mix of input audio and feedback signal
        mBuffer[mDelayIndex] = inAudio[i] + mFeedbackSampe * feedbackMapped;
        // Update feedback sample
        mFeedbackSampe = sample;
        // Write output audio
        outAudio[i] = inAudio[i] * dry + sample * wet;
        // Advance the read head
        mDelayIndex = (mDelayIndex + 1) % KAP::bufferSize;
    }
}

float KAPDelay::getInterpolatedSample (double inDelayTimeInSamples) const
{
    double readPosition = static_cast<double> (mDelayIndex) - inDelayTimeInSamples;
    
    if (readPosition < 0.0)
        readPosition += KAP::bufferSize;
    
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % KAP::bufferSize;
    
    const float readSample0 = mBuffer[readPositionIndex0];
    const float readSample1 = mBuffer[readPositionIndex1];
    
    const double fractionalReadPosition = readPosition - readPositionIndex0;
    float outSample = KAP::linearInterp(readSample0, readSample1, fractionalReadPosition);
    
    return outSample;
}

