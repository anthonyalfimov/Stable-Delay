/*
  ==============================================================================

    RBDInterfacePanel.h
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RBDInterfaceConstants.h"

class InterfacePanel  : public Component
{
public:
    InterfacePanel (ReallyBasicDelayAudioProcessor& processor);
    virtual ~InterfacePanel() = 0;
    
    void paint (Graphics& g) override;
    
protected:
    ReallyBasicDelayAudioProcessor& mProcessor;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfacePanel)
};
