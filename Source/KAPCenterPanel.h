/*
  ==============================================================================

    KAPCenterPanel.h
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"
#include "KAPCenterPanelMenuBar.h"
#include "KAPFxPanel.h"

class KAPCenterPanel  : public KAPPanelBase
{
public:
    KAPCenterPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPCenterPanel();
    
private:
    std::unique_ptr<KAPCenterPanelMenuBar> mMenuBar;
    std::unique_ptr<KAPFxPanel> mFxPanel;
};
