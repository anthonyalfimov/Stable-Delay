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
    auto& parameters = mProcessor->getParameters();
    // TODO: check whether this works on Windows!
    const RBDFxPanelStyle selectedStyle
        = static_cast<RBDFxPanelStyle> (parameters[kParameter_DelayType]->getValue());
    setFxPanelStyle (selectedStyle);
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
    
void FxPanel::setFxPanelStyle (RBDFxPanelStyle inStyle)
{
    mStyle = inStyle;
    mSliders.clear();
    
    // TODO: Can this be better implemented using std::unique_ptr or move semantics?
    
    switch (mStyle)
    {
        case kRBDFxPanelStyle_Delay:
            setName ("DELAY");
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_DelayTime],
                                                  RBDParameterLabel[kParameter_DelayTime]));
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_DelayFeedback],
                                                  RBDParameterLabel[kParameter_DelayFeedback]));
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_DelayWetDry],
                                                  RBDParameterLabel[kParameter_DelayWetDry]));
            break;
        case kRBDFxPanelStyle_Chorus:
            setName ("CHORUS");
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_ModulationRate],
                                                  RBDParameterLabel[kParameter_ModulationRate]));
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_ModulationDepth],
                                                  RBDParameterLabel[kParameter_ModulationDepth]));
            
            mSliders.add (new ParameterSlider (mProcessor->parameters,
                                                  RBDParameterID[kParameter_DelayWetDry],
                                                  RBDParameterLabel[kParameter_DelayWetDry]));
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
    
    mSliders[0]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (-1.5 * sliderSize, 15));
    mSliders[1]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (0, 15));
    mSliders[2]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (1.5 * sliderSize, 15));
    
    for (auto slider : mSliders)
    {
        addAndMakeVisible (slider);
        
        // TODO: Do we need to unregister when Sliders are destroyed?
        //       List of listeners is stored in Slider object and destroyed with it - so no?
        //
        // Register as mouse listener for Sliders so we can repaint Slider labels when mouse
        //  enters and exits Slider components
        slider->addMouseListener (this, false);
    }
    
    repaint();
}

void FxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    // TODO: Clean up usage of RBDFxPanelStyle, ComboBox index and ID:
    //       RBDFxPanelStyle <-> index <-> ID correspondense is not ensured!
    auto newStyle = static_cast<RBDFxPanelStyle> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newStyle);
}

void FxPanel::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void FxPanel::mouseExit (const MouseEvent& event)
{
    repaint();
}
