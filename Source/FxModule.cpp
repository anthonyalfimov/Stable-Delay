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

    // Set dynamic threshold detector rise and fall time constants
    mDetector.setState (detectorRiseTime, detectorFallTime);
}

void FxModule::setState (float driveInDecibels,
                         float time, float feedback, float type,
                         float modRate, float modDepth, float stereoWidth,
                         bool shouldOffsetModulation,
                         bool dynamicClipping, float /*clipRise*/, float clipFall,
                         float fbHeadroomInDb,
                         DClip::FeedbackDecayMode fbDecay, bool shouldOutputDetector,
                         float postCutFactor, DClip::CompensationMode fbComp)
{
    // Set delay input drive parameters
    mDriveSmoothed.setTargetValue (driveInDecibels);

    mUseDynamicClipping = dynamicClipping;
    mFbHeadroomGain = Decibels::decibelsToGain (fbHeadroomInDb);
    mFeedbackDecayMode = fbDecay;
    mShouldOutputDetector = shouldOutputDetector;
    mPostCutFactor = postCutFactor;
    mFbComp = fbComp;

    mFbMeter.setState (detectorRiseTime, clipFall);

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
    mDetector.prepare (sampleRate, blockSize);
    mFbMeter.prepare (sampleRate, blockSize);
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
    mDetector.reset();
    mFbMeter.reset();
    
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
        /*const*/ float feedbackGain = mFeedbackSmoothed.getNextValue();

        //============================================
        // MARK: Temporarily disable runaway feedback
        feedbackGain = jmin (feedbackGain, 1.0f);
        //============================================

        float feedbackSample = readSample * feedbackGain;

        float detectorFeedbackSample = 0.0f;

        if (mFeedbackDecayMode == DClip::Normal)
            detectorFeedbackSample = feedbackSample;
        else
            detectorFeedbackSample = readSample;

        float detectorSample = 0.0f;

        if (mFbComp == DClip::LowDrive)
            detectorSample = std::abs (writeSample
                                       + detectorFeedbackSample * mFbHeadroomGain);
        else
            detectorSample = std::abs (writeSample + detectorFeedbackSample);
                
        const float maxThreshold = -1.0f;
        const float minThreshold = -72.0f;
                
        const float gain = mDetector.processSample (detectorSample);
        const float levelInDb = Decibels::gainToDecibels (gain);
        
        const float thresholdInDb = jlimit (minThreshold, maxThreshold,
                                            levelInDb + clippingThreshold);
        const float detectorGain = Decibels::decibelsToGain (thresholdInDb);

        // Measure the feedback sample level
        const float feedbackSampleLevel
        = Decibels::gainToDecibels (mFbMeter.processSample (std::abs(feedbackSample)));
        const float feedbackRelativeLevel
        = jmax (minThreshold, feedbackSampleLevel) - thresholdInDb;
        //const float feedbackRelativeGain = Decibels::decibelsToGain (feedbackRelativeLevel);

        // Apply pre-saturator gain
        const float preBoostInDb = mDriveSmoothed.getNextValue();
        const float postCutInDb = preBoostInDb * mPostCutFactor;

        writeSample *= Decibels::decibelsToGain (preBoostInDb);
        // Drive is applied to the feedback sample, so we need to mix it in
        //  before the post-saturation gain is applied
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

        // Compensate the feedback decay by adding an appropriate amount of
        //  "dry" feedback sample
        /*const*/ float expectedPeakGain = 0.0f;

        if (mFbComp == DClip::Dynamic)
            expectedPeakGain = Decibels::decibelsToGain (postCutInDb
                                                         + feedbackRelativeLevel);
        else
            expectedPeakGain = Decibels::decibelsToGain (postCutInDb - clippingThreshold);

        if (mFeedbackDecayMode == DClip::Proportional)
            expectedPeakGain *= feedbackGain;

        if (mFbComp == DClip::LowDrive)
            expectedPeakGain /= mFbHeadroomGain;
        
        const float postClipperGain = SaturationModule::saturateBeta (expectedPeakGain);
        const float attenuationFactor = postClipperGain / expectedPeakGain;
        //DBG(attenuationFactor);
        const float compensationSample = (1.0f - attenuationFactor) * feedbackSample;

        if (mFbComp != DClip::Off)
            writeSample += compensationSample;

        // Apply post-saturator gain
        writeSample *= Decibels::decibelsToGain (-postCutInDb);

        mDelay.writeAndAdvance (writeSample);

    // WRITE OUTPUT AUDIO
        if (mShouldOutputDetector)
            outAudio[i] = detectorGain;
        else
            outAudio[i] = readSample;       // write output audio
    }
}

