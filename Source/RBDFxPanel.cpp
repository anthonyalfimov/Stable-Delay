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

    // Set up Build Version Label
    // TODO: Enable build stamp only for the debug version, or remove later
    mBuildVersionLabel.setFont (RBD::mainFont);
    mBuildVersionLabel.setJustificationType (Justification::centred);
    mBuildVersionLabel.setColour (Label::textColourId,
                                  RBD::textNormalColour.withAlpha (0.25f));
    const auto buildDate = Time::getCompilationDate();
    mBuildVersionLabel.setText ("Build: "
                                + buildDate.toString (true, true, false, true),
                                dontSendNotification);
    mBuildVersionLabel.setBorderSize ({ 1, 15, 1, 15 });

    const auto labelBounds = getLocalBounds()
                                .withSizeKeepingCentre (200, RBD::labelHeight)
                                .withBottomY (getHeight() - 7);
    mBuildVersionLabel.setBounds (labelBounds);
    addAndMakeVisible (mBuildVersionLabel);
}

FxPanel::~FxPanel()
{
    
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
                                                         Parameter::DelayFeedback));
            
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
                                                         Parameter::DelayFeedback));
            
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
        bounds.setHeight (RBD::knobSize + RBD::labelHeight);

        knob->setBounds (bounds);
        
        knobOffset += knobStep;     // update offset for the next knob
        
        addAndMakeVisible (knob);
    }

    setName (FxType::Label[mTypeIndex]);    // update panel name to reflect FX type
    mFxTypeLabel.setText (getName(), dontSendNotification); // update label text
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    const auto newTypeIndex
    = static_cast<FxType::Index> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newTypeIndex);
}
