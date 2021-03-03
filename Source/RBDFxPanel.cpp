/*
  ==============================================================================

    RBDFxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDFxPanel.h"
#include "RBDParameters.h"
#include "RBDInterfaceUtilities.h"

FxPanel::FxPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    setSize (RBD::fxPanelWidth, RBD::fxPanelHeight);

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

void FxPanel::paint (Graphics& g)
{
    InterfacePanel::paint (g);
    
    g.setColour (RBD::colour5);
    g.setFont (RBD::font3);
    g.drawText (getName(), getLocalBounds().withHeight (80), Justification::centred);
    
    for (auto knob : mKnobs)
        paintComponentLabel (g, knob);
}
    
void FxPanel::setFxPanelStyle (FxType::Index typeIndex)
{
    mTypeIndex = typeIndex;
    mKnobs.clear();
    setName (FxType::Label[mTypeIndex]);

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

        // List of listeners is stored in Knob object and destroyed with it,
        //  so we don't need to unregister this component.
        
        // Register as mouse listener for Knobs so we can repaint Knob
        //  labels when mouse enters and exits Knob components
        knob->addMouseListener (this, false);
    }
    
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    const auto newTypeIndex = static_cast<FxType::Index> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newTypeIndex);
}

void FxPanel::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void FxPanel::mouseExit (const MouseEvent& event)
{
    repaint();
}
