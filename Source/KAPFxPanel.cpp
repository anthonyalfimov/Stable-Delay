/*
  ==============================================================================

    KAPFxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPFxPanel.h"
#include "KAPParameters.h"
#include "KAPUiHelpers.h"

KAPFxPanel::KAPFxPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (FX_PANEL_WIDTH, FX_PANEL_HEIGHT);
    setFxPanelStyle (kKAPFxPanelStyle_Delay);
}


KAPFxPanel::~KAPFxPanel()
{
    
}

void KAPFxPanel::paint (Graphics& g)
{
    KAPPanelBase::paint (g);
    
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
            setName ("DelayPanel");
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayTime]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayFeedback]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayWetDry]));
            break;
        case kKAPFxPanelStyle_Chorus:
            setName ("ChorusPanel");
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_ModulationRate]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_ModulationDepth]));
            
            mSliders.add (new KAPParameterSlider (mProcessor->parameters,
                                                  KAPParameterID[kParameter_DelayWetDry]));
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
}
