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

FxPanel::FxPanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : InterfacePanel (inProcessor)
{
    setSize (RBD::fxPanelWidth, RBD::fxPanelHeight);
    const auto& parameters = mProcessor->getParameters();
    const auto selectedTypeID
    = floatToFxTypeID (parameters[Parameter::FxType]->getValue());
    setFxPanelStyle (selectedTypeID);
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
    
void FxPanel::setFxPanelStyle (FxTypeID typeID)
{
    mTypeID = typeID;
    mSliders.clear();

    switch (mTypeID)
    {
        case FxTypeID::Delay:
            setName ("DELAY");
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::DelayTime));

            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::DelayFeedback));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::DryWet));
            break;
            
        case FxTypeID::Chorus:
            setName ("CHORUS");
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::ModulationRate));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::ModulationDepth));
            
            mSliders.add (std::make_unique<ParameterSlider>
                                          (mProcessor->parameters,
                                           Parameter::DryWet));
            break;
            
        default:
            setName ("ERROR");
            jassertfalse;
            break;
    }
    
    // TODO: Positioning the sliders assumes there are always 3. Perhaps, generalise?
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
        
        // TODO: Do we need to unregister when Sliders are destroyed?
        //  List of listeners is stored in Slider object and destroyed with it - so no?
        
        // Register as mouse listener for Sliders so we can repaint Slider labels when
        //  mouse enters and exits Slider components
        slider->addMouseListener (this, false);
    }
    
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    const auto newTypeID = static_cast<FxTypeID> (comboBoxThatHasChanged->getSelectedId());
    setFxPanelStyle (newTypeID);
}

void FxPanel::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void FxPanel::mouseExit (const MouseEvent& event)
{
    repaint();
}
