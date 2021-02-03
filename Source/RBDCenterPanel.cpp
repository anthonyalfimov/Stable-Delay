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
    setSize (CENTER_PANEL_WIDTH, CENTER_PANEL_HEIGHT);
    setName ("CenterPanel");
    
    mMenuBar = std::make_unique<RBDCenterPanelMenuBar> (inProcessor);
    mMenuBar->setTopLeftPosition (0, 0);
    addAndMakeVisible (mMenuBar.get());
    
    mFxPanel = std::make_unique<RBDFxPanel> (inProcessor);
    mFxPanel->setTopLeftPosition (0, CENTER_PANEL_MENU_BAR_HEIGHT);
    addAndMakeVisible (mFxPanel.get());
    
    mMenuBar->addFxTypeComboBoxListener (mFxPanel.get());
}

RBDCenterPanel::~RBDCenterPanel()
{
    mMenuBar->removeFxTypeComboBoxListener (mFxPanel.get());
}
