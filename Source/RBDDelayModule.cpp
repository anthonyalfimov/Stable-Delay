/*
  ==============================================================================

    RBDDelayModule.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDDelayModule.h"
#include "RBDAudioUtilities.h"

DelayModule::DelayModule()
{
    reset();
}

DelayModule::~DelayModule()
{
    
}

void DelayModule::setSampleRate (double inSampleRate)
{
    mSampleRate = inSampleRate;
}

void DelayModule::reset()
{
    // Clear the feedback sample
    mFeedbackSample = 0.0f;
    // Clear the delay buffer
    zeromem (mBuffer, sizeof (float) * RBD::bufferSize);
}

void DelayModule::process (const float* inAudio,
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
    // TODO: Why not use NormalizableRange<> for parameters instead of mapping them here?
    const float timeMapped = jmap (inTime, 0.001f, 1.0f);
    float feedbackMapped = jmap (inFeedback, 0.0f, 0.95f);
    
    for (int i = 0; i < inNumSamplesToRender; ++i)
    {
        // TODO: Can FX type be changed on the block level?
        //       Or will this lead to audible delay before FX type switches
        
        if (static_cast<RBDDelayType> (inType) == kRBDDelayType_Delay)
        {
            // Use member variable to maintain consistent smoothing between blocks
            mTimeSmoothed = mTimeSmoothed - RBD::paramSmoothingCoefFine * (mTimeSmoothed - timeMapped);
        }
        else
        {
            // TODO: Switching FX to CHORUS produces a chirp. Should we fix this?
            //       The chirp is due to smoothed delay time change. Slower smoothing
            //       will fix this. Perhaps, slower smoothing just for the chorus?
            
            const double delayTimeModulated = 0.003 + 0.002 * inModulationBuffer[i];
            
            // Use member variable to maintain consistent smoothing between blocks
            mTimeSmoothed = mTimeSmoothed - RBD::paramSmoothingCoefFine * (mTimeSmoothed - delayTimeModulated);
            
            // Reset feedback to 0
            feedbackMapped = 0.0f;
        }
        
        const double delayTimeInSamples = mTimeSmoothed * mSampleRate;
        const float sample = getInterpolatedSample (delayTimeInSamples);
        
        // Write to the buffer: mix of input audio and feedback signal
        mBuffer[mWritePosition] = inAudio[i] + mFeedbackSample * feedbackMapped;
        // Update feedback sample
        mFeedbackSample = sample;
        // Write output audio
        outAudio[i] = inAudio[i] * dry + sample * wet;
        // Advance the read head
        mWritePosition = (mWritePosition + 1) % RBD::bufferSize;
    }
}

float DelayModule::getInterpolatedSample (double delayTimeInSamples) const
{
    double readPosition = static_cast<double> (mWritePosition) - delayTimeInSamples;
    
    if (readPosition < 0.0)
        readPosition += RBD::bufferSize;
    
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % RBD::bufferSize;
    
    const float readSample0 = mBuffer[readPositionIndex0];
    const float readSample1 = mBuffer[readPositionIndex1];
    
    const double fractionalReadPosition = readPosition - readPositionIndex0;
    float outSample = RBD::linearInterp (readSample0, readSample1, fractionalReadPosition);
    
    return outSample;
}

