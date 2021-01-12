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
    g.setColour (Colours::whitesmoke);
    g.fillAll();
    
    g.setColour (Colours::black);
    g.drawRect (0, 0, getWidth(), getHeight(), 2);
    
    g.drawFittedText (getName(), getLocalBounds(), Justification::centred, 1);
}
