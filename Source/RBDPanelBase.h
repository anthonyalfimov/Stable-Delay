/*
  ==============================================================================

    RBDPanelBase.h
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RBDInterfaceDefines.h"

// TODO: turn into an abstract base class

class RBDPanelBase  : public Component
{
public:
    // TODO: switch to passing Processor by reference for consistency with JUCE
    RBDPanelBase (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDPanelBase();
    
    void paint (Graphics& g) override;
    
protected:
    ReallyBasicDelayAudioProcessor* mProcessor;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDPanelBase)
};
