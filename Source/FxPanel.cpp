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

    // Set up FX Type ComboBox
    mFxTypeDropdown = std::make_unique<ParameterDropdown> (mProcessor.parameters,
                                                           Parameter::FxType,
                                                           RadioGroup::FxType,
                                                           FxType::Label);

    // MARK: Remove to re-enable keyboard control
    mFxTypeDropdown->setWantsKeyboardFocus (false);

    const int comboBoxWidth = 300;
    const int comboBoxHeight = 50;
    auto bounds = getLocalBounds().withSizeKeepingCentre (comboBoxWidth, comboBoxHeight);
    bounds.setY (20);
    mFxTypeDropdown->setBounds (bounds);
    mFxTypeDropdown->setJustificationType (Justification::centred);
    addAndMakeVisible (mFxTypeDropdown.get());
    mFxTypeDropdown->addListener (this);

    // Set up initial Panel Style
    auto* fxTypeParameter = dynamic_cast<AudioParameterFloat*>
    (mProcessor.parameters.getParameter (Parameter::ID[Parameter::FxType]));

    if (fxTypeParameter == nullptr)
        jassertfalse;

    const auto selectedTypeIndex = static_cast<FxType::Index> (fxTypeParameter->get());
    setFxPanelStyle (selectedTypeIndex);
}
    
void FxPanel::setFxPanelStyle (FxType::Index typeIndex)
{
    mTypeIndex = typeIndex;
    mKnobs.clear();

    // Add controls based on the FxType
    switch (mTypeIndex)
    {
        case FxType::Delay:
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::DelayTime));

            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::Feedback,
                                                         Parameter::InvertFeedback));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::DryWet));
            break;

        case FxType::Chorus:
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::ModulationRate));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::ModulationDepth));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::DryWet));
            break;
            
        case FxType::Flanger:
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::ModulationRate));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::ModulationDepth));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::Feedback,
                                                         Parameter::InvertFeedback));
            
            mKnobs.add (std::make_unique<ParameterKnob> (mProcessor.parameters,
                                                         Parameter::DryWet));
            break;
            
        default:
            setName ("ERROR");
            jassertfalse;
            break;
    }
    
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

    setName (FxType::Label[mTypeIndex]);    // update panel name to reflect FX type
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    const auto newTypeIndex
    = static_cast<FxType::Index> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newTypeIndex);
}
