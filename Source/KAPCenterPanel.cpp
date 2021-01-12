/*
  ==============================================================================

    KAPCenterPanel.cpp
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPCenterPanel.h"

KAPCenterPanel::KAPCenterPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (CENTER_PANEL_WIDTH, CENTER_PANEL_HEIGHT);
    setName ("CenterPanel");
}

KAPCenterPanel::~KAPCenterPanel()
{
    
}
