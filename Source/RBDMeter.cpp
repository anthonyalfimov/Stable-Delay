/*
  ==============================================================================

    RBDMeter.cpp
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDMeter.h"
#include "RBDInterfaceConstants.h"

Meter::Meter (Parameter::Index parameterIndex,
              ReallyBasicDelayAudioProcessor& processor)
    : mParameterIndex (parameterIndex)
{
    // Initialise meter probe pointers
    switch (mParameterIndex)
    {
        case Parameter::InputGain:
            mNumChannels = processor.getTotalNumInputChannels();
            mMeterProbes.resize (mNumChannels);

            for (int i = 0; i < mNumChannels; ++i)
                mMeterProbes[i] = processor.getInputMeterProbe (i);

            break;

        case Parameter::OutputGain:
            mNumChannels = processor.getTotalNumOutputChannels();
            mMeterProbes.resize (mNumChannels);

            for (int i = 0; i < mNumChannels; ++i)
                mMeterProbes[i] = processor.getOutputMeterProbe (i);

            break;

        default:
            jassertfalse;
            mNumChannels = 0;
            mMeterProbes.clear();
            break;
    }

    jassert (mNumChannels > 0);
    jassert (mNumChannels == 2);    // can only handle 2 channels at the moment

    // Wake up meter probes
    for (auto& meterProbe : mMeterProbes)
    {
        jassert (meterProbe != nullptr);
        meterProbe->setSuspended (false);
    }

    // Release smoothing is required for proper meter painting:
    //  check that the number of smoothing steps is not zero
    jassert (meterReleaseTime * refreshRate >= 1.0f);

    // Initialise peak levels
    mPeakLevelsInDb.resize (mNumChannels);

    for (auto& peakLevel : mPeakLevelsInDb)
    {
        peakLevel.reset (refreshRate, meterReleaseTime);
        peakLevel.setCurrentAndTargetValue (minLevelInDb);
    }

    // Initialise RMS levels
    mRmsLevelsInDb.resize (mNumChannels);

    for (auto& rmsLevel : mRmsLevelsInDb)
    {
        rmsLevel.reset (refreshRate, meterReleaseTime);
        rmsLevel.setCurrentAndTargetValue (minLevelInDb);
    }

    startTimerHz (refreshRate);  // start the timer
}

Meter::~Meter()
{
    // Suspend meter probes
    for (auto& meterProbe : mMeterProbes)
        meterProbe->setSuspended (true);
}

void Meter::paint (Graphics& g)
{
    jassert (mNumChannels == 1 || mNumChannels == 2);   // can handle 1 or 2 channels

    const auto meterBounds = getLocalBounds();

    // Meter bounds are created on the left
    auto channelBounds = meterBounds.withWidth (RBD::meterChannelWidth);

    // Position the mono meter in the centre
    if (mNumChannels == 1)
        channelBounds.setCentre (meterBounds.getCentre());

    for (int i = 0; i < mNumChannels; ++i)
    {
        // Position the right stereo meter on the right
        if (i == 1)
            channelBounds.setX (meterBounds.getRight() - channelBounds.getWidth());

        // Fill meter background
        g.setColour (RBD::meterBgColour);
        g.fillRoundedRectangle (channelBounds.toFloat(), RBD::defaultCornerSize);

        // Draw peak level meter bar
        //      NB: Smoothing is advanced in the timer callback, here we just
        //      get the current value
        const float peakPosition
        = meterRange.convertTo0to1 (mPeakLevelsInDb[i].getCurrentValue());
        int barTop = static_cast<int> (meterBounds.getBottom()
                                       - meterBounds.getHeight() * peakPosition);
        auto barBounds = channelBounds.withTop (barTop);

        const int delta = 2;    // reduction amount for the meter bar size

        // Keep corners consistent by changing their radius by the same amount
        //  as the size of the object itself
        const float barCornerSize = RBD::defaultCornerSize - delta;

        // Peak meter fill
        g.setColour (RBD::meterFillColour.withAlpha (0.2f));
        g.fillRoundedRectangle (barBounds.reduced (delta).toFloat(),
                                barCornerSize);

        // Peak meter outline

        // Rectangle outline can still be visible when bounds are reduced below
        //  their size (inverted), so explicitly check if we should paint it
        if (barBounds.getHeight() > 2 * delta)
        {
            g.setColour (RBD::meterFillColour.withAlpha (0.7f));
            g.drawRoundedRectangle (barBounds.toFloat().reduced (delta + 0.5f),
                                    barCornerSize, 1.0f);
        }

        // Draw RMS level meter bar
        //      NB: Smoothing is advanced in the timer callback, here we just
        //      get the current value
        const float rmsPosition
        = meterRange.convertTo0to1 (mRmsLevelsInDb[i].getCurrentValue());
        barTop = static_cast<int> (meterBounds.getBottom()
                                   - meterBounds.getHeight() * rmsPosition);
        barBounds.setTop (barTop);

        // RMS meter fill
        g.setColour (RBD::meterFillColour);
        g.fillRoundedRectangle (barBounds.reduced (delta).toFloat(),
                                barCornerSize);
    }
}

void Meter::timerCallback()
{
    bool isSignalPreset = false;    // flag to check if significant signal detected
    bool isSmoothing = false;       // flag to check if levels are smoothing

    for (int i = 0; i < mNumChannels; ++i)
    {
        // Retrieve peak level
        const float peakLevel = mMeterProbes[i]->getPeakLevel()->load();
        mMeterProbes[i]->resetPeakLevel();  // reset stored max peak level

        float peakLevelInDb = Decibels::gainToDecibels (peakLevel);

        if (peakLevelInDb > minLevelInDb)
        {
            /* If we need the actual peak level, this is the place to grab it */

            // Clamp to meter range
            peakLevelInDb = jmin (peakLevelInDb, maxLevelInDb);

            // Immidiately jump to a higher peak, but ramp to a lower one
            if (peakLevelInDb > mPeakLevelsInDb[i].getCurrentValue())
                mPeakLevelsInDb[i].setCurrentAndTargetValue (peakLevelInDb);
            else
                mPeakLevelsInDb[i].setTargetValue (peakLevelInDb);

            isSignalPreset = true;
        }
        else
        {
            mPeakLevelsInDb[i].setTargetValue (minLevelInDb);
        }

        if (mPeakLevelsInDb[i].isSmoothing())
        {
            isSmoothing = true; // indicate that there's a new value to be shown
            mPeakLevelsInDb[i].getNextValue();  // generate next smoothed value
        }

        // Retrieve RMS level
        const float rmsLevel = mMeterProbes[i]->getRmsLevel()->load();
        float rmsLevelInDb = Decibels::gainToDecibels (rmsLevel);

        if (rmsLevelInDb > minLevelInDb)
        {
            /* If we need the actual peak level, this is the place to grab it */

            // Clamp to meter range
            rmsLevelInDb = jmin (rmsLevelInDb, maxLevelInDb);

            // Immediately jump to a higher value, but ramp to a lower one
            if (rmsLevelInDb > mRmsLevelsInDb[i].getCurrentValue())
                mRmsLevelsInDb[i].setCurrentAndTargetValue (rmsLevelInDb);
            else
                mRmsLevelsInDb[i].setTargetValue (rmsLevelInDb);

            isSignalPreset = true;
        }
        else
        {
            mRmsLevelsInDb[i].setTargetValue (minLevelInDb);
        }

        if (mRmsLevelsInDb[i].isSmoothing())
        {
            isSmoothing = true; // indicate that there's a new value to be shown
            mRmsLevelsInDb[i].getNextValue();   // generate next smoothed value
        }
    }

    // Only repaint if there is significant level present in at least one of
    //  the meters, or at least one of the meters is still smoothing and
    //  there's a new value to be shown

    if (isSignalPreset || isSmoothing)
        repaint();
}
