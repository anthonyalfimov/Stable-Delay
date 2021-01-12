/*
  ==============================================================================

    KAPFxPanel.cpp
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPFxPanel.h"

KAPFxPanel::KAPFxPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (FX_PANEL_WIDTH, FX_PANEL_HEIGHT);
    updateName();
}


KAPFxPanel::~KAPFxPanel()
{
    
}
    
void KAPFxPanel::setFxPanelStyle (KAPFxPanelStyle inStyle)
{
    mStyle = inStyle;
    updateName();
}
    
void KAPFxPanel::updateName()
{
    switch (mStyle)
    {
        case kKAPFxPanelStyle_Delay:
            setName ("DelayPanel");
            break;
            
        case kKAPFxPanelStyle_Chorus:
            setName ("ChorusPanel");
            break;
            
        default:
            // This should never be reached
            setName ("ERROR");
            jassertfalse;
            break;
    }
}
