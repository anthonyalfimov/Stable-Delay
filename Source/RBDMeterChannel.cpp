/*
  ==============================================================================

    RBDMeterChannel.cpp
    Created: 24 Mar 2021 3:52:12pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDMeterChannel.h"
#include "RBDInterfaceConstants.h"

MeterChannel::MeterChannel (MeterProbe* meterProbe, int refreshRate)
    : mMeterProbe (meterProbe), mRefreshRate (refreshRate)
{
    jassert (mMeterProbe != nullptr);
    if (mMeterProbe != nullptr)
        mMeterProbe->setSuspended (false);

    // Release smoothing is required for proper meter painting:
    //  check that the number of smoothing steps is not zero
    jassert (meterReleaseTime * mRefreshRate >= 1.0f);

    // Initialise peak level
    mPeakLevelInDb.reset (refreshRate, meterReleaseTime);
    mPeakLevelInDb.setCurrentAndTargetValue (minLevelInDb);

    // Initialise RMS level
    mRmsLevelInDb.reset (refreshRate, meterReleaseTime);
    mRmsLevelInDb.setCurrentAndTargetValue (minLevelInDb);
}

MeterChannel::~MeterChannel()
{
    // Suspend the meter probe
    if (mMeterProbe != nullptr)
        mMeterProbe->setSuspended (true);
}

void MeterChannel::setStyle (MeterStyle newStyle)
{
    mStyle = newStyle;
    resetClippingIndicator();
}

void MeterChannel::resetClippingIndicator()
{
    mShowClippingIndicator = false;
    repaint();
}

void MeterChannel::update()
{
    if (mMeterProbe == nullptr)
        return;

    bool isSignalPreset = false;    // flag to check if significant signal detected
    bool isSmoothing = false;       // flag to check if levels are smoothing

//==== Update Peak Level reading

    // Retrieve peak level value
    const float peakGain = mMeterProbe->getPeakLevel()->load();
    mMeterProbe->resetPeakLevel();  // reset stored max peak level
    float peakLevelInDb = Decibels::gainToDecibels (peakGain);

    // Update smoothed peak level value
    if (peakLevelInDb > minLevelInDb)
    {
        // Immidiately jump to a higher peak, but ramp to a lower one
        if (peakLevelInDb > mPeakLevelInDb.getCurrentValue())
            mPeakLevelInDb.setCurrentAndTargetValue (peakLevelInDb);
        else
            mPeakLevelInDb.setTargetValue (peakLevelInDb);

        isSignalPreset = true;
    }
    else
    {
        mPeakLevelInDb.setTargetValue (minLevelInDb);
    }

    if (mPeakLevelInDb.isSmoothing())
    {
        isSmoothing = true; // indicate that there's a new value to be shown
        mPeakLevelInDb.getNextValue();  // generate next smoothed value
    }

//==== Update RMS Level reading

    // Retrieve RMS level value
    const float rmsLevel = mMeterProbe->getRmsLevel()->load();
    mMeterProbe->resetRmsLevel();   // reset RMS level in case plugin is bypassed
    float rmsLevelInDb = Decibels::gainToDecibels (rmsLevel);

    // Update smoothed RMS level value
    if (rmsLevelInDb > minLevelInDb)
    {
        // Immediately jump to a higher value, but ramp to a lower one
        if (rmsLevelInDb > mRmsLevelInDb.getCurrentValue())
            mRmsLevelInDb.setCurrentAndTargetValue (rmsLevelInDb);
        else
            mRmsLevelInDb.setTargetValue (rmsLevelInDb);

        isSignalPreset = true;
    }
    else
    {
        mRmsLevelInDb.setTargetValue (minLevelInDb);
    }

    if (mRmsLevelInDb.isSmoothing())
    {
        isSmoothing = true; // indicate that there's a new value to be shown
        mRmsLevelInDb.getNextValue();   // generate next smoothed value
    }

    // Only repaint if there is significant level present in at least one of
    //  the meters, or at least one of the meters is still smoothing and
    //  there's a new value to be shown
    if (isSignalPreset || isSmoothing)
        repaint();
}

void MeterChannel::paint (Graphics& g)
{
    const auto channelBounds = getLocalBounds().reduced (padding);

    auto fillColour = RBD::meterFillColour;

    // Retrieve peak level
    //    NB: Smoothing is advanced in the update() method that is called by
    //    the parent's timer callback. Here we just get the current value.
    float peakLevel = mPeakLevelInDb.getCurrentValue();

    // Handle clipping / saturation indication
    switch (mStyle)
    {
        case MeterStyle::Clipping:
            if (peakLevel > 0.0f)
            {
                fillColour = RBD::meterClippingColour;
                mShowClippingIndicator = true;
            }

            if (mShowClippingIndicator)
            {
                const int delta = 1;    // expansion amount
                // Keep corners consistent by changing their radius by the same
                //  amount as the size of the object itself
                const float cornerSize = RBD::defaultCornerSize + delta;
                g.setColour (RBD::meterClippingColour);

                g.drawRoundedRectangle (channelBounds.toFloat()
                                                     .expanded (delta - 0.5f),
                                        cornerSize, 1.0f);
            }

            break;

        case MeterStyle::Saturation:
        {
            // Saturation threshold is picked semi-arbitrarily
            float saturationAmount = jmap (peakLevel, -3.0f, 0.0f, 0.0f, 1.0f);
            saturationAmount = jlimit (0.0f, 1.0f, saturationAmount);
            fillColour = fillColour.interpolatedWith (RBD::meterSaturationColour,
                                                      saturationAmount);
            break;
        }

        default:
            break;
    }

    // Fill meter background
    g.setColour (RBD::meterBgColour);
    g.fillRoundedRectangle (channelBounds.toFloat(), RBD::defaultCornerSize);

    // Draw peak level meter bar
    peakLevel = jmin (peakLevel, maxLevelInDb); // clamp to meter range
    const float peakPosition = range.convertTo0to1 (peakLevel);
    int barTop = static_cast<int> (channelBounds.getBottom()
                                   - channelBounds.getHeight() * peakPosition);
    auto barBounds = channelBounds.withTop (barTop);

    const int delta = 2;    // reduction amount for the meter bar size

    // Keep corners consistent by changing their radius by the same amount
    //  as the size of the object itself
    const float barCornerSize = RBD::defaultCornerSize - delta;

    // Peak meter fill
    g.setColour (fillColour.withAlpha (0.2f));
    g.fillRoundedRectangle (barBounds.reduced (delta).toFloat(), barCornerSize);

    // Peak meter outline

    // Rectangle outline can still be visible when bounds are reduced below
    //  their size (inverted), so explicitly check if we should paint it
    if (barBounds.getHeight() > 2 * delta)
    {
        g.setColour (fillColour.withAlpha (0.7f));
        g.drawRoundedRectangle (barBounds.toFloat().reduced (delta + 0.5f),
                                barCornerSize, 1.0f);
    }

    // Draw RMS level meter bar
    //    NB: Smoothing is advanced in the update() method that is called by
    //    the parent's timer callback. Here we just get the current value.
    float rmsLevel = mRmsLevelInDb.getCurrentValue();
    rmsLevel = jmin (rmsLevel, maxLevelInDb);   // clamp to meter range
    const float rmsPosition = range.convertTo0to1 (rmsLevel);
    barTop = static_cast<int> (channelBounds.getBottom()
                               - channelBounds.getHeight() * rmsPosition);
    barBounds.setTop (barTop);

    // RMS meter fill
    g.setColour (fillColour);
    g.fillRoundedRectangle (barBounds.reduced (delta).toFloat(),
                            barCornerSize);
}

