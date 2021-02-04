/*
  ==============================================================================

    RBDMainPanel.h
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDPanelBase.h"
#include "RBDTopPanel.h"
#include "RBDGainPanel.h"
#include "RBDCentrePanel.h"

class RBDMainPanel  : public RBDPanelBase
{
public:
    RBDMainPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDMainPanel();
    
private:
    std::unique_ptr<RBDTopPanel> mTopPanel;
    std::unique_ptr<RBDGainPanel> mInputGainPanel;
    std::unique_ptr<RBDGainPanel> mOutputGainPanel;
    std::unique_ptr<RBDCentrePanel> mCentrePanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDMainPanel)
};
