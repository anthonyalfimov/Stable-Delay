/*
  ==============================================================================

    RBDGainPanel.cpp
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDGainPanel.h"

GainPanel::GainPanel (ReallyBasicDelayAudioProcessor& processor,
                      Parameter::Index parameterIndex)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::gainPanelWidth, RBD::gainPanelHeight);
    setName ("GainPanel");

    // Set up Gain knob
    mKnob = std::make_unique<ParameterKnob> (mProcessor.parameters,
                                             parameterIndex);

    const int knobSize = RBD::defaultKnobSize;
    mKnob->setBounds (getLocalBounds().withSizeKeepingCentre (knobSize, knobSize)
                                      .withY (20));
    addAndMakeVisible (mKnob.get());

    // Set up Gain knob label
    mLabel = std::make_unique<SliderLabel> (mKnob.get());
    addAndMakeVisible (mLabel.get());

    // Set up Level Meters
    int numChannels = 0;
    MeterStyle meterStyle = MeterStyle::Normal;

    switch (parameterIndex)
    {
        case Parameter::InputGain:
            numChannels = processor.getTotalNumInputChannels();
            meterStyle = MeterStyle::Normal;
            break;

        case Parameter::OutputGain:
            numChannels = processor.getTotalNumOutputChannels();
            meterStyle = MeterStyle::Clipping;
            break;

        default:
            break;
    }

    // Can only handle mono and stereo channel setup
    jassert (numChannels == 1 || numChannels == 2);

    mMeter = std::make_unique<Meter> (parameterIndex, mProcessor, numChannels);
    mMeter->setStyle (meterStyle);

    int meterWidth = 0; // no meters visible unless mono or stereo channel config

    if (numChannels == 1)
        meterWidth = RBD::meterChannelWidth;    // set up for single meter
    else if (numChannels == 2)
        meterWidth = 3 * RBD::meterChannelWidth;    // set up for two meters with a gap

    // Set meter width to 3x width of a meter channel
    const int meterGap = 20;
    auto meterBounds = mKnob->getBounds().withSizeKeepingCentre (meterWidth, 0);
    meterBounds.setTop (mKnob->getBottom() + RBD::labelHeight + meterGap);
    meterBounds.setBottom (getHeight() - meterGap);
    // Add padding to accomodate clipping indicators
    meterBounds.expand (Meter::padding, Meter::padding);
    mMeter->setBounds (meterBounds);
    addAndMakeVisible (mMeter.get());
}

GainPanel::~GainPanel()
{
    
}
