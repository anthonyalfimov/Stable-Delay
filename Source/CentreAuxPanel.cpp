/*
  ==============================================================================

    CentreAuxPanel.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "CentreAuxPanel.h"
#include "Parameters.h"

CentreAuxPanel::CentreAuxPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::centrePanelMenuBarWidth, RBD::centrePanelMenuBarHeight);
    setName ("CentreAuxPanel");
}
