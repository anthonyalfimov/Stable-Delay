/*
  ==============================================================================

    FxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "FxPanel.h"
#include "Parameters.h"
#include "PluginLookAndFeel.h"

FxPanel::FxPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::fxPanelWidth, RBD::fxPanelHeight);

    mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                 Parameter::DClipRise));

    mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                 Parameter::DClipFall));

    mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                 Parameter::DryWet));

    const float knobPadding = 0.5f;
    const float knobStep = 1.0f + knobPadding;

    // Calculate leftmost knob offset first:
    float knobOffset = -knobStep * (mKnobs.size() - 1) / 2.0f;

    // NB: not using auto& here because the contained objects are pointers,
    //  so it's ok to work with copies
    for (auto knob : mKnobs)
    {
        auto bounds = getLocalBounds()
                            .withSizeKeepingCentre (RBD::knobSize, RBD::knobSize)
                            .translated (knobOffset * RBD::knobSize, 15.0f);

        if (knob->hasToggle())
            bounds.setHeight (RBD::knobSize + RBD::labelHeight + RBD::toggleHeight);
        else
            bounds.setHeight (RBD::knobSize + RBD::labelHeight);

        knob->setBounds (bounds);

        knobOffset += knobStep;     // update offset for the next knob

        addAndMakeVisible (knob);
    }
}
