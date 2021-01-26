/*
  ==============================================================================

    KAPPanelBase.cpp
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPPanelBase.h"

KAPPanelBase::KAPPanelBase (KadenzeAudioPluginAudioProcessor* inProcessor)
    : mProcessor (inProcessor)
{
    
}

KAPPanelBase::~KAPPanelBase()
{
    
}

void KAPPanelBase::paint (Graphics& g)
{
    g.setColour (Colours::black);
    g.fillAll();
    
    g.setColour (Colours::whitesmoke);
    g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (1.0f), 4.0f);
    
    // g.drawFittedText (getName(), getLocalBounds().withTrimmedTop (10), Justification::centredTop, 1);
}
