/*
  ==============================================================================

    RBDCenterPanel.cpp
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCenterPanel.h"

RBDCenterPanel::RBDCenterPanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : RBDPanelBase (inProcessor)
{
    setSize (RBD::centrePanelWidth, RBD::centrePanelWidth);
    setName ("CenterPanel");
    
    mMenuBar = std::make_unique<RBDCenterPanelMenuBar> (inProcessor);
    mMenuBar->setTopLeftPosition (0, 0);
    addAndMakeVisible (mMenuBar.get());
    
    mFxPanel = std::make_unique<RBDFxPanel> (inProcessor);
    mFxPanel->setTopLeftPosition (0, mMenuBar->getBottom());
    addAndMakeVisible (mFxPanel.get());
    
    mMenuBar->addFxTypeComboBoxListener (mFxPanel.get());
}

RBDCenterPanel::~RBDCenterPanel()
{
    mMenuBar->removeFxTypeComboBoxListener (mFxPanel.get());
}
