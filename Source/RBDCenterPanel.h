/*
  ==============================================================================

    RBDCenterPanel.h
    Created: 11 Jan 2021 4:32:18pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDPanelBase.h"
#include "RBDCenterPanelMenuBar.h"
#include "RBDFxPanel.h"

class RBDCenterPanel  : public RBDPanelBase
{
public:
    RBDCenterPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDCenterPanel();
    
private:
    std::unique_ptr<RBDCenterPanelMenuBar> mMenuBar;
    std::unique_ptr<RBDFxPanel> mFxPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDCenterPanel)
};
