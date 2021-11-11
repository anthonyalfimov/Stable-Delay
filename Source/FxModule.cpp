/*
  ==============================================================================

    FxModule.cpp
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#include "FxModule.h"
#include "AudioConstants.h"

FxModule::FxModule()
{
    // Set the saturation curve
    mSaturator.setState (SaturationCurve::beta);
    
    // Disable hold for input and feedback detectors
    mInputDetector.setHold (false);
    mFeedbackDetector.setHold (false);
}

void FxModule::setState (float driveInDecibels,
                         float time, float feedback, float type,
                         float modRate, float modDepth, float stereoWidth,
                         bool shouldOffsetModulation,
                         bool dynamicClipping,
                         float detRise, float detFall, float clipFall,
                         bool shouldOutputDetector,
                         float postCutFactor)
{
    // Set delay input drive parameters
    mDriveSmoothed.setTargetValue (driveInDecibels);
    
    mInputDetector.setState (detRise, detFall);
    mFeedbackDetector.setState (detRise, detFall);
    mThresholdDetector.setState (detectorRiseTime, clipFall);

    mUseDynamicClipping = dynamicClipping;
    mShouldOutputDetector = shouldOutputDetector;
    mPostCutFactor = postCutFactor;

    // Set delay and modulation parameters
    mTypeValue = static_cast<FxType::Index> (type);

    float modAmplitude = 0.0f;

    switch (mTypeValue)
    {
        case FxType::Delay:
        {
            mTimeSmoothed.setTargetValue (time / 1000.0);   // convert from ms to s
            mFeedbackSmoothed.setTargetValue (feedback / 100.0f);    // convert from %

            // TODO: Consider using PiecewiseNormalisableRange to map delay modulation

            modRate = delayModRate; // set fixed slow modulation
            // Set the max modulation amplitude to 0.1 the delay time - as long
            //  as it fits between min and max values for delay time modulation
            //  amlitude
            modAmplitude = (stereoWidth / 100.0f) * jlimit (minDelayTimeAmplitude,
                                                            maxDelayTimeAmplitude,
                                                            time / 10000.0f);
            break;
        }

        case FxType::Chorus:
        {
            mTimeSmoothed.setTargetValue (chorusCentreTime);
            mFeedbackSmoothed.setTargetValue (0.0f);    // disable feedback
            modAmplitude = chorusTimeAmplitude * modDepth / 100.0f;  // convert from %
            break;
        }

        case FxType::Flanger:
        {
            mTimeSmoothed.setTargetValue (flangerCentreTime);
            mFeedbackSmoothed.setTargetValue (feedback / 100.0f); // convert from %
            modAmplitude = flangerTimeAmplitude * modDepth / 100.0f; // convert from %
            break;
        }

        default:
        {
            jassertfalse;
            break;
        }
    }

    const float modOffset = shouldOffsetModulation ? stereoWidth : 0.0f;
    mLfo.setState (modRate, modAmplitude, modOffset);
}

void FxModule::prepare (double sampleRate, int blockSize)
{
    DspModule::prepare (sampleRate, blockSize);

    // Prepare contained modules
    mInputDetector.prepare (sampleRate, blockSize);
    mFeedbackDetector.prepare (sampleRate, blockSize);
    mThresholdDetector.prepare (sampleRate, blockSize);
    mSaturator.prepare (sampleRate, blockSize);
    mDelay.prepare (sampleRate, blockSize);
    mLfo.prepare (sampleRate, blockSize);

    // Allocate modulation buffer
    mModulationBuffer = std::make_unique<float[]> (mBlockSize);
    
    reset();
}

void FxModule::reset()
{
    // Clear the modulation buffer
    if (mModulationBuffer != nullptr)
        FloatVectorOperations::clear (mModulationBuffer.get(), mBlockSize);

    // Reset contained modules
    mInputDetector.reset();
    mFeedbackDetector.reset();
    mThresholdDetector.reset();
    
    mSaturator.reset();
    mDelay.reset();
    mLfo.reset();

    // Reset smoothed parameters
    mTimeSmoothed.reset (mSampleRate, 0.1);
    mFeedbackSmoothed.reset (mSampleRate, 0.05);
    mDriveSmoothed.reset (mSampleRate, 0.0);
}

void FxModule::process (const float* inAudio, float* outAudio,
                        int numSamplesToRender)
{
    // FILL MODULATION BUFFER
    mLfo.process (mModulationBuffer.get(), mModulationBuffer.get(), numSamplesToRender);

    for (int i = 0; i < numSamplesToRender; ++i)
    {
    // READ SAMPLE FROM THE DELAY BUFFER
        // MARK: Enforce double precision
        const double delayTimeInSeconds = mTimeSmoothed.getNextValue()
                                          + mModulationBuffer[i];
        const float readSample = mDelay.read (delayTimeInSeconds);

        // We must assume that inAudio and outAudio can point to the same location.
        // Therefore, we must finish reading data from inAudio before writing
        // to outAudio.

    // WRITE SAMPLE TO THE DELAY BUFFER AND ADVANCE THE WRITE HEAD
        float writeSample = inAudio[i];
        
        const float feedbackGain = mFeedbackSmoothed.getNextValue();
        float feedbackSample = readSample * feedbackGain;

        const float inputLevelGain
        = mInputDetector.processSample (std::abs (writeSample));
        const float feedbackLevelGain
        = mFeedbackDetector.processSample (std::abs (feedbackSample));
     
        // Enable hold when feedback level is above input level
        mThresholdDetector.setHold (feedbackLevelGain > inputLevelGain);
        
        const float gain = mThresholdDetector.processSample (std::abs (writeSample));
        const float levelInDb = Decibels::gainToDecibels (gain);
        
        const float thresholdInDb = jlimit (minThreshold, maxThreshold,
                                            levelInDb + thresholdDelta);
        const float detectorGain = Decibels::decibelsToGain (thresholdInDb);

        // Apply pre-saturator gain
        const float preBoostInDb = mDriveSmoothed.getNextValue();
        const float postCutInDb = preBoostInDb * mPostCutFactor;

        writeSample *= Decibels::decibelsToGain (preBoostInDb);
        feedbackSample *= Decibels::decibelsToGain (postCutInDb);

        // Add feedback sample
        writeSample += feedbackSample;
        
        // Apply threshold-move boost
        if (mUseDynamicClipping)
            writeSample *= Decibels::decibelsToGain (-thresholdInDb);

        // Pass the signal through the saturator before writing it to the buffer
        mSaturator.process (&writeSample, &writeSample, 1);

        // Apply threshold-move cut
        if (mUseDynamicClipping)
            writeSample *= Decibels::decibelsToGain (thresholdInDb);

        // Apply post-saturator gain
        writeSample *= Decibels::decibelsToGain (-postCutInDb);
        
        // Compensate the feedback decay by adding an appropriate amount of
        //  "dry" feedback sample
//        const float expectedPeakGain = Decibels::decibelsToGain (postCutInDb - clippingThreshold);
//
//        const float postClipperGain = SaturationModule::saturateBeta (expectedPeakGain);
//        const float attenuationFactor = postClipperGain / expectedPeakGain;
//        const float compensationSample = (1.0f - attenuationFactor) * feedbackSample;
//        writeSample += compensationSample;

        mDelay.writeAndAdvance (writeSample);

    // WRITE OUTPUT AUDIO
        if (mShouldOutputDetector)
            outAudio[i] = detectorGain;
        else
            outAudio[i] = readSample;       // write output audio
    }
}

