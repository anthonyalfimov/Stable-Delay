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
#include "RBDParameters.h"

DelayModule::DelayModule()
{
    reset();
}

DelayModule::~DelayModule()
{
    
}

void DelayModule::setSampleRate (double sampleRate)
{
    mSampleRate = sampleRate;
    mBufferSize = mSampleRate * maxDelayTimeInSeconds;
    mBuffer = std::make_unique<float[]> (mBufferSize);
    reset();
}

void DelayModule::reset()
{
    // Clear the feedback sample
    mFeedbackSample = 0.0f;
    // Clear the delay buffer
    if (mBuffer != nullptr)
        zeromem (mBuffer.get(), sizeof (float) * mBufferSize);
}

void DelayModule::process (const float* inAudio,
                           float time,
                           float feedback,
                           float dryWet,
                           float type,
                           const float* modulationBuffer,
                           float* outAudio,
                           int numSamplesToRender)
{
    const float wet = dryWet;
    const float dry = 1.0f - wet;
    
    // TODO: Use NormalizableRange<> for parameters instead of mapping them here
    const float timeMapped = jmap (time, 0.001f, maxDelayTimeInSeconds);
    float feedbackMapped = jmap (feedback, 0.0f, 0.95f);
    
    // TODO: Parameter values currently can only change on block level. Adjust accordingly
    //  We load the value from the parameter in the plugin processor. Here we
    //  are working with a copy that can't change. So we should change the code
    //  accordingly.
    //  Alternatively, we could pass the ptr to the parameter to the DSP modules
    //  and load values every sample to handle parameter change on sample level.
    
    for (int i = 0; i < numSamplesToRender; ++i)
    {
        const auto typeID = floatToFxTypeID (type);
        
        if (typeID == FxTypeID::Delay)
        {
            // Use member variable to maintain consistent smoothing between blocks
            mTimeSmoothed = mTimeSmoothed - RBD::paramSmoothingCoefFine * (mTimeSmoothed - timeMapped);
        }
        else
        {
            // TODO: Switching FX to CHORUS produces a chirp. Should we fix this?
            //  The chirp is due to smoothed delay time change. Slower smoothing
            //  will fix this. Perhaps, slower smoothing just for the chorus?
            
            const double delayTimeModulated = 0.003 + 0.002 * modulationBuffer[i];
            
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
        mWritePosition = (mWritePosition + 1) % mBufferSize;
    }
}

float DelayModule::getInterpolatedSample (double delayTimeInSamples) const
{
    double readPosition = static_cast<double> (mWritePosition) - delayTimeInSamples;
    
    if (readPosition < 0.0)
        readPosition += mBufferSize;
    
    const int readPositionIndex0 = static_cast<int> (readPosition);
    const int readPositionIndex1 = (readPositionIndex0 + 1) % mBufferSize;
    
    const float readSample0 = mBuffer[readPositionIndex0];
    const float readSample1 = mBuffer[readPositionIndex1];
    
    const double fractionalReadPosition = readPosition - readPositionIndex0;
    float outSample = RBD::linearInterp (readSample0, readSample1, fractionalReadPosition);
    
    return outSample;
}

