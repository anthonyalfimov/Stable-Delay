/*
  ==============================================================================

    InterfacePanel.h
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "InterfaceConstants.h"

// TODO: Consider storing a reference to APVTS instead of the processor
//  Right now the processor reference is used to:
//  - Access parameter values (can be done through APVTS)
//  - Get access to Preset Manager
//  - Create the Level Meter object (could we communicate level through APVTS?)
//  To do this, we need to somehow deal with the last two points

class InterfacePanel  : public Component
{
public:
    InterfacePanel (ReallyBasicDelayAudioProcessor& processor);
    virtual ~InterfacePanel() = 0;
    
    void paint (Graphics& g) override;
    
protected:
    ReallyBasicDelayAudioProcessor& mProcessor;
    
private:
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfacePanel)
};
