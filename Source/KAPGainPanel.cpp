/*
  ==============================================================================

    KAPGainPanel.cpp
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPGainPanel.h"

KAPGainPanel::KAPGainPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (GAIN_PANEL_WIDTH, GAIN_PANEL_HEIGHT);
    setName ("GainPanel");
}

KAPGainPanel::~KAPGainPanel()
{
    
}

void KAPGainPanel::setParameterID (KAPParameter inParameter)
{
    mSlider = std::make_unique<KAPParameterSlider> (mProcessor->parameters,
                                                    KAPParameterID[inParameter]);
    
    const int sliderSize = 54;
    mSlider->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize));
    
    addAndMakeVisible (mSlider.get());
}
