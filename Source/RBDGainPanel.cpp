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

    // Set up Level Meter
    mMeter = std::make_unique<LevelMeter> (parameterIndex, mProcessor);

    const int meterWidth = 56;
    const int meterGap = 20;
    auto meterBounds = mKnob->getBounds().withSizeKeepingCentre (meterWidth, 0);
    meterBounds.setTop (mKnob->getBottom() + RBD::labelHeight + meterGap);
    meterBounds.setBottom (getHeight() - meterGap);
    mMeter->setBounds (meterBounds);
    addAndMakeVisible (mMeter.get());
}

GainPanel::~GainPanel()
{
    
}
