/*
  ==============================================================================

    KAPMainPanel.cpp
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPMainPanel.h"

KAPMainPanel::KAPMainPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
}

KAPMainPanel::~KAPMainPanel()
{
    
}
