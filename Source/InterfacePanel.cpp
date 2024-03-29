/*
  ==============================================================================

    InterfacePanel.cpp
    Created: 11 Jan 2021 4:31:58pm
    Author:  Anthony

  ==============================================================================
*/

#include "InterfacePanel.h"

InterfacePanel::InterfacePanel (ReallyBasicDelayAudioProcessor& processor)
    : mProcessor (processor)
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
