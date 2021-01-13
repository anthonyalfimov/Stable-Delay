/*
  ==============================================================================

    KAPFxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"

// TODO: switch to an enum class, put the enum inside the class

enum KAPFxPanelStyle
{
    kKAPFxPanelStyle_Delay = 0,
    kKAPFxPanelStyle_Chorus,
    kKAPFxPanelStyle_TotalNumStyles
};

class KAPFxPanel  : public KAPPanelBase
{
public:
    KAPFxPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPFxPanel();
    
    void setFxPanelStyle (KAPFxPanelStyle inStyle);
    
private:
    void updateName();
    
    KAPFxPanelStyle mStyle = kKAPFxPanelStyle_Delay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPFxPanel)
};
