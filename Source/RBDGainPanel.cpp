/*
  ==============================================================================

    RBDGainPanel.cpp
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDGainPanel.h"

GainPanel::GainPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    setSize (RBD::gainPanelWidth, RBD::gainPanelHeight);
    setName ("GainPanel");
}

GainPanel::~GainPanel()
{
    
}

void GainPanel::setParameterID (Parameter::Index parameterIndex)
{
//  CREATE KNOB
    mKnob = std::make_unique<ParameterKnob> (mProcessor.parameters,
                                             parameterIndex);
    
    const int knobSize = RBD::defaultKnobSize;
    
    mKnob->setBounds (getLocalBounds().withSizeKeepingCentre (knobSize, knobSize)
                                      .withY (20));
    
    addAndMakeVisible (mKnob.get());
    
//  CREATE KNOB LABEL
    mLabel = std::make_unique<SliderLabel> (mKnob.get());
    
    addAndMakeVisible (mLabel.get());
    
//  CREATE METER
    mMeter = std::make_unique<LevelMeter> (parameterIndex, mProcessor);
    
    // Define meter bounds
    const int meterWidth = 56;
    const int meterGap = 20;
    Rectangle<int> meterBounds
    = mKnob->getBounds().withSizeKeepingCentre (meterWidth, 0);
    meterBounds.setTop (mKnob->getBottom() + RBD::labelHeight + meterGap);
    meterBounds.setBottom (getHeight() - meterGap);
    mMeter->setBounds (meterBounds);
    
    addAndMakeVisible (mMeter.get());
}
