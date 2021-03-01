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
    
    for (auto slider : mSliders)
        paintComponentLabel (g, slider);
}
    
void FxPanel::setFxPanelStyle (FxType::Index typeIndex)
{
    mTypeIndex = typeIndex;
    mSliders.clear();
    setName (FxType::Label[mTypeIndex]);

    // Add controls based on the FxType
    switch (mTypeIndex)
    {
        case FxType::Delay:
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::DelayTime));

            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::DelayFeedback));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::DryWet));
            break;

        //  Chorus and Flanger use the same set of controls
        case FxType::Chorus:
        case FxType::Flanger:
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::ModulationRate));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::ModulationDepth));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor.parameters,
                                           Parameter::DryWet));
            break;
            
        default:
            setName ("ERROR");
            jassertfalse;
            break;
    }
    
    // TODO: Positioning the sliders assumes there are always 3. Generalise
    if (mSliders.size() != 3)
        jassertfalse;           // array must contain 3 sliders to continue
    
    const int sliderSize = 80;
    
    mSliders[0]->setBounds (getLocalBounds()
                            .withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (-1.5 * sliderSize, 15));
    mSliders[1]->setBounds (getLocalBounds()
                            .withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (0, 15));
    mSliders[2]->setBounds (getLocalBounds()
                            .withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (1.5 * sliderSize, 15));
    
    for (auto slider : mSliders)
    {
        addAndMakeVisible (slider);

        // List of listeners is stored in Slider object and destroyed with it,
        //  so we don't need to unregister this component.
        
        // Register as mouse listener for Sliders so we can repaint Slider
        //  labels when mouse enters and exits Slider components
        slider->addMouseListener (this, false);
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
