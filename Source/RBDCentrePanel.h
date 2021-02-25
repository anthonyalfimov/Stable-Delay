/*
  ==============================================================================

    RBDCentrePanel.h
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDCentrePanelMenuBar.h"
#include "RBDFxPanel.h"

class CentrePanel  : public InterfacePanel
{
public:
    CentrePanel (ReallyBasicDelayAudioProcessor& processor);
    ~CentrePanel();
    
private:
    std::unique_ptr<CentrePanelMenuBar> mMenuBar;
    std::unique_ptr<FxPanel> mFxPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentrePanel)
};
