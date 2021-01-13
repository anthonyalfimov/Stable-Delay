/*
  ==============================================================================

    KAPGainPanel.h
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"

class KAPGainPanel  : public KAPPanelBase
{
public:
    KAPGainPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPGainPanel();
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPGainPanel)
};
