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
    for (auto meterProbe : mMeterProbes)
    {
        jassert (meterProbe != nullptr);
        meterProbe->setSuspended (false);
    }

    // Initialise meter values
    mRmsLevelsInDb.resize (mNumChannels);
    std::fill (mRmsLevelsInDb.begin(), mRmsLevelsInDb.end(), minLevelInDb);

    mPeakLevelsInDb.resize (mNumChannels);
    std::fill (mPeakLevelsInDb.begin(), mPeakLevelsInDb.end(), minLevelInDb);

    startTimerHz (refreshRate);  // start the timer
}

Meter::~Meter()
{
    // Suspend meter probes
    for (auto meterProbe : mMeterProbes)
        meterProbe->setSuspended (true);
}

void Meter::paint (Graphics& g)
{
    jassert (mNumChannels == 1 || mNumChannels == 2);   // can handle only 1 or 2 channels

    // Split component width into 3 equal parts:
    //  two channels - left meter, gap, right meter
    //  one channel  - gap, meter, gap
    const int meterWidth = getWidth() / 3;
    auto meterBounds = getLocalBounds().withWidth (meterWidth);

    if (mNumChannels == 1)
        meterBounds.setX (meterWidth);

    for (int i = 0; i < mNumChannels; ++i)
    {
        meterBounds.setX (i * 2 * meterWidth);  // position the meter

        // Fill meter background
        g.setColour (RBD::meterBgColour);
        g.fillRoundedRectangle (meterBounds.toFloat(), RBD::defaultCornerSize);

        // Draw peak level meter bar
        float peakPosition = meterRange.convertTo0to1 (mPeakLevelsInDb[i]);
        int barTop = static_cast<int> (getHeight() * (1.0f - peakPosition));
        auto barBounds = meterBounds.withTop (barTop);

        g.setColour (RBD::meterFillColour);
        g.drawRoundedRectangle (barBounds.reduced (2).toFloat(),
                                RBD::defaultCornerSize, 1.0f);

        // Draw RMS level meter bar
        float rmsPosition = meterRange.convertTo0to1 (mRmsLevelsInDb[i]);
        barTop = static_cast<int> (getHeight() * (1.0f - rmsPosition));
        barBounds.setTop (barTop);

        g.setColour (RBD::meterFillColour);
        g.fillRoundedRectangle (barBounds.reduced (2).toFloat(),
                                RBD::defaultCornerSize);
    }
}

void Meter::timerCallback()
{
    bool isSignalPreset = false;    // flag to check if significant signal detected

    for (int i = 0; i < mNumChannels; ++i)
    {
        // Retrieve peak level
        const float peakLevel = mMeterProbes[i]->getPeakLevel()->load();
        mMeterProbes[i]->resetPeakLevel();  // reset stored max peak level

        const float peakLevelInDb = Decibels::gainToDecibels (peakLevel);

        if (peakLevelInDb > minLevelInDb)
        {
            // If we need the actual peak level, this is the place to grab it
            mPeakLevelsInDb[i] = jmin (peakLevelInDb, maxLevelInDb);
            isSignalPreset = true;
        }
        else
        {
            mPeakLevelsInDb[i] = minLevelInDb;
        }

        // Retrieve RMS level
        const float rmsLevel = mMeterProbes[i]->getRmsLevel()->load();
        const float rmsLevelInDb = Decibels::gainToDecibels (rmsLevel);

        if (rmsLevelInDb > minLevelInDb)
        {
            // If we need the actual peak level, this is the place to grab it
            mRmsLevelsInDb[i] = jmin (rmsLevelInDb, maxLevelInDb);
            isSignalPreset = true;
        }
        else
        {
            mRmsLevelsInDb[i] = minLevelInDb;
        }
    }

    // If no signal is present in the meters, repaint once to clear the meters
    //  and make sure they don't get stuck on the previous non-zero value.
    //  After that, unset mShouldRepaint to not keep repainting empty meters.

    if (isSignalPreset || mShouldRepaint)
        repaint();

    mShouldRepaint = isSignalPreset;
}
