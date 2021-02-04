/*
  ==============================================================================

    RBDInterfacePanel.cpp
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDInterfacePanel.h"

InterfacePanel::InterfacePanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : mProcessor (inProcessor)
{
    
}

// Pure virtual destructor still requires an empty body
InterfacePanel::~InterfacePanel()
{
    
}

void InterfacePanel::paint (Graphics& g)
{
    // Common paint actions for all panels go here
}
