/*
  ==============================================================================

    KAPFxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPFxPanel.h"
#include "KAPParameters.h"
#include "KAPInterfaceHelpers.h"

KAPFxPanel::KAPFxPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (FX_PANEL_WIDTH, FX_PANEL_HEIGHT);
    // TODO: check whether this works on Windows!
    const KAPFxPanelStyle selectedStyle
        = static_cast<KAPFxPanelStyle> (mProcessor->getParameter (kParameter_DelayType));
    setFxPanelStyle (selectedStyle);
    
    setRepaintsOnMouseActivity (true);
}

KAPFxPanel::~KAPFxPanel()
{
    
}

void KAPFxPanel::paint (Graphics& g)
{
    KAPPanelBase::paint (g);
    
    g.setColour (KAP::colour5);
    g.setFont (KAP::font3);
    g.drawText (getName(), getLocalBounds().withHeight (80), Justification::centred);
    
    for (auto slider : mSliders)
        paintComponentLabel (g, slider);
}
    
void KAPFxPanel::setFxPanelStyle (KAPFxPanelStyle inStyle)
{
    mStyle = inStyle;
    mSliders.clear();
    
    const int sliderSize = 56;
    
    // TODO: Can this be better implemented using std::unique_ptr or move semantics?
    
    switch (mStyle)
    {
        case kKAPFxPanelStyle_Delay:
            setName ("DELAY");
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayTime],
                                                  KAPParameterLabel[kParameter_DelayTime]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayFeedback],
                                                  KAPParameterLabel[kParameter_DelayFeedback]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayWetDry],
                                                  KAPParameterLabel[kParameter_DelayWetDry]));
            break;
        case kKAPFxPanelStyle_Chorus:
            setName ("CHORUS");
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_ModulationRate],
                                                  KAPParameterLabel[kParameter_ModulationRate]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_ModulationDepth],
                                                  KAPParameterLabel[kParameter_ModulationDepth]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayWetDry],
                                                  KAPParameterLabel[kParameter_DelayWetDry]));
            break;
            
        default:
            setName ("ERROR");
            jassertfalse;
            break;
    }
    
    // TODO: Positioning the sliders assumes there are always 3. Perhaps, generalise?
    if (mSliders.size() != 3)
        jassertfalse;           // array must contain 3 sliders to continue
    
    mSliders[0]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (-2 * sliderSize, 0));
    mSliders[1]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize));
    mSliders[2]->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize)
                            .translated (2 * sliderSize, 0));
    
    for (auto slider : mSliders)
        addAndMakeVisible (slider);
    
    repaint();
}

void KAPFxPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    // TODO: Clean up usage of KAPFxPanelStyle, ComboBox index and ID:
    //       KAPFxPanelStyle <-> index <-> ID correspondense is not ensured!
    auto newStyle = static_cast<KAPFxPanelStyle> (comboBoxThatHasChanged->getSelectedItemIndex());
    setFxPanelStyle (newStyle);
}
