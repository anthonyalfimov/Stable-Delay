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

    // Set input and feedback detector time constants
    mInputDetector.setState (detectorRiseTime, detectorFallTime);
    mFeedbackDetector.setState (detectorRiseTime, detectorFallTime);
}

void FxModule::setState (float driveInDecibels,
                         float time, float feedback, bool invert, float type,
                         float modRate, float modDepth, float stereoWidth,
                         bool shouldOffsetModulation,
                         bool dynamicClipping,
                         float limRise, float limConstFall, float limFallRange,
                         bool shouldOutputDetector,
                         float postCutFactor,
                         float fbMaxHeadroom, float fbMinHeadroom, float fbOverdrive)
{
    // Set delay input drive parameters
    mDriveSmoothed.setTargetValue (driveInDecibels);

    mFeedbackLimitDetectorRise = limRise;
    mFeedbackLimitDetectorFallConst = limConstFall;
    mFeedbackLimitDetectorFallRange = limFallRange;

    mFeedbackMaxHeadroom = fbMaxHeadroom;
    mFeedbackMinHeadroom = fbMinHeadroom;
    mFeedbackOverdrive = fbOverdrive;

    mUseDynamicClipping = dynamicClipping;
    mShouldOutputDetector = shouldOutputDetector;
    mPostCutFactor = postCutFactor;

    // Set delay and modulation parameters
    mTypeValue = static_cast<FxType::Index> (type);

    const float feedbackSign = invert ? -1.0f : 1.0f;
    float modAmplitude = 0.0f;
    

    switch (mTypeValue)
    {
        case FxType::Delay:
        {
            // Convert from ms to s:
            mTimeSmoothed.setTargetValue (time / 1000.0);
            // Convert from % (include feedback sign for smoothing):
            mFeedbackSmoothed.setTargetValue (feedbackSign * feedback / 100.0f);

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
            // Convert from % (include feedback sign for smoothing):
            mFeedbackSmoothed.setTargetValue (feedbackSign * feedback / 100.0f);
            // Convert from %:
            modAmplitude = flangerTimeAmplitude * modDepth / 100.0f;
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
    mFeedbackLimitDetector.prepare (sampleRate, blockSize);
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
    mFeedbackLimitDetector.reset();
    
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
        const float feedbackParameterValue = mFeedbackSmoothed.getNextValue();

        // Temporarily negate feedback value sign:
        const float feedbackSign = (feedbackParameterValue < 0) ? -1.0f : 1.0f;
        float feedbackAbsValue = std::abs (feedbackParameterValue);

        float feedbackHeadroom = mFeedbackMaxHeadroom;
        float driveCompensation = feedbackAbsValue;

        if (feedbackAbsValue > 1.0f)
        {
            const float feedbackExcess = feedbackAbsValue - 1.0f;
            feedbackAbsValue = 1.0f + feedbackExcess * mFeedbackOverdrive;

            driveCompensation = 1.0f;

            feedbackHeadroom = mFeedbackMaxHeadroom
                - 5.0f * feedbackExcess * (mFeedbackMaxHeadroom - mFeedbackMinHeadroom);
        }

        float feedbackSample = readSample * feedbackSign * feedbackAbsValue;

        // Enable hold when feedback is set to 100% or more
        mFeedbackLimitDetector.setHold (feedbackAbsValue >= 1.0f);

        // Update limit detector time constants based on feedback
        const float feedbackLimitDetectorFall = mFeedbackLimitDetectorFallConst
        + mFeedbackLimitDetectorFallRange * jmin (1.0f, feedbackAbsValue);
        mFeedbackLimitDetector.setState (mFeedbackLimitDetectorRise,
                                         feedbackLimitDetectorFall);

        // Compute individual level envelopes for input and feedback
        float inputLevelGain
        = mInputDetector.processSample (std::abs (writeSample));
        const float feedbackRawLevelGain
        = mFeedbackDetector.processSample (std::abs (feedbackSample));
        
        // Determine feedback level limit
        float feedbackLimitGain
        = mFeedbackLimitDetector.processSample (std::abs (writeSample));

        // Adjust headroom for limited feedback
        const float headroomAdjustmentGain
            = Decibels::decibelsToGain (feedbackHeadroom - clipperHeadroom);
        feedbackLimitGain *= headroomAdjustmentGain;

        // Adjust headroom for input signal
        inputLevelGain *= headroomAdjustmentGain;

        // Limit feedback level
        float feedbackLimitedLevelGain = jmin (feedbackRawLevelGain, feedbackLimitGain);

        // Calculate pre-saturation gain levels (for compensation)
        const float preBoostInDb = mDriveSmoothed.getNextValue();
        const float postCutInDb = preBoostInDb * mPostCutFactor;

        // Compensate Drive for feedback signal
        const float driveCompensationInDb = postCutInDb * driveCompensation;
        feedbackLimitedLevelGain *= Decibels::decibelsToGain (driveCompensationInDb);

        // Calculate the final level envelope
        const float gain = jmax (inputLevelGain, feedbackLimitedLevelGain);
        const float levelInDb = Decibels::gainToDecibels (gain);
        const float thresholdInDb = jlimit (minThreshold, maxThreshold,
                                            levelInDb + clipperHeadroom);
        
        //======================================================================
        //  TMP: For threshold output
        const float thresholdGain = Decibels::decibelsToGain (thresholdInDb);
        //======================================================================

        // Apply pre-saturator gain
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

        mDelay.writeAndAdvance (writeSample);

    // WRITE OUTPUT AUDIO
        if (mShouldOutputDetector)
            outAudio[i] = thresholdGain;
        else
            outAudio[i] = readSample;       // write output audio
    }
}

