/*
  ==============================================================================

    MainPanel.h
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "TopPanel.h"
#include "GainPanel.h"
#include "CentrePanel.h"

class MainPanel  : public InterfacePanel
{
public:
    MainPanel (ReallyBasicDelayAudioProcessor& processor);
    ~MainPanel();
    
private:
    std::unique_ptr<TopPanel> mTopPanel;
    std::unique_ptr<GainPanel> mInputGainPanel;
    std::unique_ptr<GainPanel> mOutputGainPanel;
    std::unique_ptr<CentrePanel> mCentrePanel;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};
