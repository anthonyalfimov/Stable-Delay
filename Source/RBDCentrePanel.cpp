/*
  ==============================================================================

    RBDCentrePanel.cpp
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCentrePanel.h"

CentrePanel::CentrePanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : InterfacePanel (inProcessor)
{
    setSize (RBD::centrePanelWidth, RBD::centrePanelWidth);
    setName ("CenterPanel");
    
    mMenuBar = std::make_unique<CentrePanelMenuBar> (inProcessor);
    mMenuBar->setTopLeftPosition (0, 0);
    addAndMakeVisible (mMenuBar.get());
    
    mFxPanel = std::make_unique<FxPanel> (inProcessor);
    mFxPanel->setTopLeftPosition (0, mMenuBar->getBottom());
    addAndMakeVisible (mFxPanel.get());
    
    mMenuBar->addFxTypeComboBoxListener (mFxPanel.get());
}

CentrePanel::~CentrePanel()
{
    mMenuBar->removeFxTypeComboBoxListener (mFxPanel.get());
}
