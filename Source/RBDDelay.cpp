/*
  ==============================================================================

    RBDDelay.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDDelay.h"
#include "RBDAudioUtilities.h"

RBDDelay::RBDDelay()
{
    reset();
}

RBDDelay::~RBDDelay()
{
    
}

void RBDDelay::setSampleRate (double inSampleRate)
{
    mSampleRate = inSampleRate;
}

void RBDDelay::reset()
{
    // Clear the feedback sample
    mFeedbackSample = 0.0f;
    // Clear the delay buffer
    zeromem (mBuffer, sizeof (float) * RBD::bufferSize);
}

// FIXME: Delay behaves incorrectly when Time set to 0 - remap min Delay Time or fix behaviour

void RBDDelay::process (const float* inAudio,
                        float inTime,
                        float inFeedback,
                        float inWetDry,
                        float inType,
                        const float* inModulationBuffer,
                        float* outAudio,
                        int inNumSamplesToRender)
{
    const float wet = inWetDry;
    const float dry = 1.0f - wet;
    /*const*/ float feedbackMapped = jmap (inFeedback, 0.0f, 1.0f, 0.0f, 0.95f);
    
    for (int i = 0; i < inNumSamplesToRender; ++i)
    {
        // TODO: does this need to be done on sample level? Maybe better to set type on block level?
        
        if (static_cast<RBDDelayType> (inType) == kRBDDelayType_Delay)
        {
            // Use member variable to maintain consistent smoothing between blocks
            mTimeSmoothed = mTimeSmoothed - RBD::paramSmoothingCoefFine * (mTimeSmoothed - inTime);
        }
        else
        {
            const double delayTimeModulated = 0.003 + 0.002 * inModulationBuffer[i];
            
            // Use member variable to maintain consistent smoothing between blocks
            mTimeSmoothed = mTimeSmoothed - RBD::paramSmoothingCoefFine * (mTimeSmoothed - delayTimeModulated);
            
            // TODO: better feedback reset - currently it has audible trail before disappearing
            // Reset feedback to 0
            feedbackMapped = 0.0f;
        }
        
        const double delayTimeInSamples = mTimeSmoothed * mSampleRate;
        const float sample = getInterpolatedSample (delayTimeInSamples);
        
        // Write to the buffer: mix of input audio and feedback signal
        mBuffer[mDelayIndex] = inAudio[i] + mFeedbackSample * feedbackMapped;
        // Update feedback sample
        mFeedbackSample = sample;
        // Write output audio
        outAudio[i] = inAudio[i] * dry + sample * wet;
        // Advance the read head
        mDelayIndex = (mDelayIndex + 1) % RBD::bufferSize;
    }
}

float RBDDelay::getInterpolatedSample (double inDelayTimeInSamples) const
{
    double readPosition = static_cast<double> (mDelayIndex) - inDelayTimeInSamples;
    
    if (readPosition < 0.0)
        readPosition += RBD::bufferSize;
    
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % RBD::bufferSize;
    
    const float readSample0 = mBuffer[readPositionIndex0];
    const float readSample1 = mBuffer[readPositionIndex1];
    
    const double fractionalReadPosition = readPosition - readPositionIndex0;
    float outSample = RBD::linearInterp(readSample0, readSample1, fractionalReadPosition);
    
    return outSample;
}

