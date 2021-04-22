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
              ReallyBasicDelayAudioProcessor& processor,
              int numChannels)
    : mNumChannels (numChannels)
{
    // Can only handle mono and stereo channel setup
    jassert (mNumChannels == 1 || mNumChannels == 2);

    // Create MeterLegend
    auto labelLevels = {0, -8, -16, -24, -32};
    mMeterLegend = std::make_unique<MeterLegend> (labelLevels, mNumChannels == 2);
    addAndMakeVisible (mMeterLegend.get());
    mMeterLegend->addMouseListener (this, true);

    // Create and initialise MeterChannel objects
    switch (parameterIndex)
    {
        case Parameter::InputGain:
            for (int i = 0; i < mNumChannels; ++i)
            {
                auto* meterProbe = processor.getInputMeterProbe (i);
                mMeterChannels.add (std::make_unique<MeterChannel> (meterProbe,
                                                                    refreshRate));
            }

            break;

        case Parameter::OutputGain:
            for (int i = 0; i < mNumChannels; ++i)
            {
                auto* meterProbe = processor.getOutputMeterProbe (i);
                mMeterChannels.add (std::make_unique<MeterChannel> (meterProbe,
                                                                    refreshRate));
            }

            break;

        default:
            jassertfalse;
            break;
    }

    for (auto meterChannel : mMeterChannels)
    {
        addAndMakeVisible (meterChannel);
        meterChannel->addMouseListener (this, false);
    }

    // Start meter refreshing
    startTimerHz (refreshRate);  // start the timer
}

Meter::~Meter()
{
    mMeterLegend->removeMouseListener (this);

    for (auto meterChannel : mMeterChannels)
        meterChannel->removeMouseListener (this);
}

void Meter::setStyle (MeterStyle newStyle)
{
    mStyle = newStyle;

    // NB: not using auto& here because the contained objects are pointers,
    //  so it's ok to work with copies
    for (auto meterChannel : mMeterChannels)
        meterChannel->setStyle (newStyle);
}

void Meter::resized()
{
    // Position MeterChannel objects
    const int paddedWidth = RBD::meterChannelWidth + 2 * MeterChannel::padding;

    // Can only handle mono and stereo channel setup
    //  (other channel configurations will not have visible meters)
    if (mNumChannels == 1)
    {
        // Position the single mono meter in the centre on gain panel
        auto bounds = getLocalBounds().withWidth (paddedWidth);
        mMeterChannels[0]->setBounds (bounds);

        // Position the MeterLegend on the right of the MeterChannel
        bounds.setX (mMeterChannels[0]->getRight() - MeterChannel::padding);
        bounds.setRight (getWidth() - Meter::padding);
        // Reduce the height of MeterLegend to match the height of the actual
        //  MeterChannel bar without the padding.
        bounds.reduce (0, MeterChannel::padding + MeterChannel::inset);
        mMeterLegend->setBounds (bounds);
    }
    else if (mNumChannels == 2)
    {
        // Position the left meter on the left
        auto bounds = getLocalBounds().withWidth (paddedWidth);
        mMeterChannels[0]->setBounds (bounds);
        // Position the right meter on the right
        bounds.setX (getWidth() - paddedWidth);
        mMeterChannels[1]->setBounds (bounds);

        // Position the MeterLegend in between the channels
        bounds.setX (mMeterChannels[0]->getRight());
        bounds.setRight (mMeterChannels[1]->getX());
        // Expand the width of MeterLegend to ignore the MeterChannel padding,
        //  and reduce the height to match the height of actual MeterChannel
        //  bars without the padding.
        bounds.expand (MeterChannel::padding,
                       -(MeterChannel::padding + MeterChannel::inset));
        mMeterLegend->setBounds (bounds);
    }
}

void Meter::timerCallback()
{
    // NB: not using auto& here because the contained objects are pointers,
    //  so it's ok to work with copies
    for (auto meterChannel : mMeterChannels)
        meterChannel->update();
}

void Meter::mouseDown (const MouseEvent& /*event*/)
{
    // NB: not using auto& here because the contained objects are pointers,
    //  so it's ok to work with copies
    for (auto meterChannel : mMeterChannels)
        meterChannel->resetClippingIndicator();
}
