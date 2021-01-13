/*
  ==============================================================================

    KAPCenterPanelMenuBar.h
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"

class KAPCenterPanelMenuBar  : public KAPPanelBase
{
public:
    KAPCenterPanelMenuBar (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPCenterPanelMenuBar();
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPCenterPanelMenuBar)
};
