/*
  ==============================================================================

    KAPPanelBase.h
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KAPInterfaceDefines.h"

// TODO: turn into an abstract base class

class KAPPanelBase  : public Component
{
public:
    KAPPanelBase (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPPanelBase();
    
    void paint (Graphics& g) override;
    
protected:
    KadenzeAudioPluginAudioProcessor* mProcessor;
};
