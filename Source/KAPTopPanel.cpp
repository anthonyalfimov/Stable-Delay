/*
  ==============================================================================

    KAPTopPanel.cpp
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPTopPanel.h"

KAPTopPanel::KAPTopPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (TOP_PANEL_WIDTH, TOP_PANEL_HEIGHT);
}

KAPTopPanel::~KAPTopPanel()
{
    
}

void KAPTopPanel::paint (Graphics& g)
{
    KAPPanelBase::paint (g);

    g.setFont (Font ("Gill Sans", 16.0f, Font::plain));
    g.drawFittedText ("KADENZE AUDIO PLUGIN", getBounds().withTrimmedRight (10), Justification::centredRight, 1);
}
