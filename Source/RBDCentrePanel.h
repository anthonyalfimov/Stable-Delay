/*
  ==============================================================================

    RBDCenterPanel.h
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDPanelBase.h"
#include "RBDCentrePanelMenuBar.h"
#include "RBDFxPanel.h"

class RBDCentrePanel  : public RBDPanelBase
{
public:
    RBDCentrePanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDCentrePanel();
    
private:
    std::unique_ptr<RBDCentrePanelMenuBar> mMenuBar;
    std::unique_ptr<RBDFxPanel> mFxPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDCentrePanel)
};
