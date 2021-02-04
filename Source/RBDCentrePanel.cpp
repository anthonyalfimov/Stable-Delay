/*
  ==============================================================================

    RBDCenterPanel.cpp
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCentrePanel.h"

RBDCentrePanel::RBDCentrePanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : RBDPanelBase (inProcessor)
{
    setSize (RBD::centrePanelWidth, RBD::centrePanelWidth);
    setName ("CenterPanel");
    
    mMenuBar = std::make_unique<RBDCentrePanelMenuBar> (inProcessor);
    mMenuBar->setTopLeftPosition (0, 0);
    addAndMakeVisible (mMenuBar.get());
    
    mFxPanel = std::make_unique<RBDFxPanel> (inProcessor);
    mFxPanel->setTopLeftPosition (0, mMenuBar->getBottom());
    addAndMakeVisible (mFxPanel.get());
    
    mMenuBar->addFxTypeComboBoxListener (mFxPanel.get());
}

RBDCentrePanel::~RBDCentrePanel()
{
    mMenuBar->removeFxTypeComboBoxListener (mFxPanel.get());
}
