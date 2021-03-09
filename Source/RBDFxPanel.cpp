/*
  ==============================================================================

    RBDFxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDFxPanel.h"
#include "RBDParameters.h"

FxPanel::FxPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::fxPanelWidth, RBD::fxPanelHeight);

    // Set up the FX Type Label
    mFxTypeLabel.setFont (RBD::fxTypeFont);
    mFxTypeLabel.setJustificationType (Justification::centred);
    mFxTypeLabel.setColour (Label::textColourId, RBD::textFxTypeColour);
    mFxTypeLabel.setBounds (getLocalBounds().withHeight (80));
    addAndMakeVisible (mFxTypeLabel);

    // Set up initial Panel Style
    auto* fxTypeParameter = dynamic_cast<AudioParameterFloat*>
    (mProcessor.parameters.getParameter (Parameter::ID[Parameter::FxType]));

    if (fxTypeParameter == nullptr)
        jassertfalse;

    const auto selectedTypeIndex = static_cast<FxType::Index> (fxTypeParameter->get());
    setFxPanelStyle (selectedTypeIndex);
}

FxPanel::~FxPanel()
{
    
}
    
void FxPanel::setFxPanelStyle (FxType::Index typeIndex)
{
    mTypeIndex = typeIndex;
    mLabels.clear();
    mKnobs.clear();

    // Add controls based on the FxType
    switch (mTypeIndex)
    {
        case FxType::Delay:
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DelayTime));

            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DelayFeedback));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DryWet));
            break;

        case FxType::Chorus:
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::ModulationRate));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::ModulationDepth));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DryWet));
            break;
            
        case FxType::Flanger:
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::ModulationRate));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::ModulationDepth));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DelayFeedback));
            
            mKnobs.add (std::make_unique<ParameterKnob>
                                          (mProcessor.parameters,
                                           Parameter::DryWet));
            break;
            
        default:
            setName ("ERROR");
            jassertfalse;
            break;
    }
    
    const int knobSize = RBD::defaultKnobSize;
    const float knobPadding = 0.5f;
    const float knobStep = 1.0f + knobPadding;
    
    // Calculate leftmost knob offset first:
    float knobOffset = -knobStep * (mKnobs.size() - 1) / 2.0f;
        
    for (auto knob : mKnobs)
    {
        knob->setBounds (getLocalBounds()
                           .withSizeKeepingCentre (knobSize, knobSize)
                           .translated (knobOffset * knobSize, 15.0f));
        
        knobOffset += knobStep;     // update offset for the next knob
        
        addAndMakeVisible (knob);
        addAndMakeVisible (mLabels.add (std::make_unique<SliderLabel> (knob)));
    }

    setName (FxType::Label[mTypeIndex]);    // update panel name to reflect FX type
    mFxTypeLabel.setText (getName(), dontSendNotification); // update label text
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    const auto newTypeIndex = static_cast<FxType::Index> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newTypeIndex);
}
