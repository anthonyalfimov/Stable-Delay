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
    // Create and initialise MeterChannel objects
    switch (mParameterIndex)
    {
        case Parameter::InputGain:
            mNumChannels = processor.getTotalNumInputChannels();

            for (int i = 0; i < mNumChannels; ++i)
            {
                auto* meterProbe = processor.getInputMeterProbe (i);
                mMeterChannels.add (std::make_unique<MeterChannel> (meterProbe,
                                                                    refreshRate));
            }

            break;

        case Parameter::OutputGain:
            mNumChannels = processor.getTotalNumOutputChannels();

            for (int i = 0; i < mNumChannels; ++i)
            {
                auto* meterProbe = processor.getOutputMeterProbe (i);
                mMeterChannels.add (std::make_unique<MeterChannel> (meterProbe,
                                                                    refreshRate));
            }

            break;

        default:
            jassertfalse;
            mNumChannels = 0;
            break;
    }

    // Can only handle mono and stereo channel setup
    jassert (mNumChannels > 0 && mNumChannels <= 2);

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

    if (mNumChannels == 1)
    {
        // Position the single mono meter in the centre
        auto bounds = getLocalBounds().withSizeKeepingCentre (paddedWidth,
                                                              getHeight());
        mMeterChannels[0]->setBounds (bounds);
    }
    else if (mNumChannels == 2)
    {
        // Position the left meter on the left
        auto bounds = getLocalBounds().withWidth (paddedWidth);
        mMeterChannels[0]->setBounds (bounds);
        // Position the right meter on the right
        DBG(getRight());
        bounds.setX (getWidth() - paddedWidth);
        mMeterChannels[1]->setBounds (bounds);
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
