/*
  ==============================================================================

    RBDMainPanel.cpp
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDMainPanel.h"
#include "RBDParameters.h"

RBDMainPanel::RBDMainPanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : RBDPanelBase (inProcessor)
{
    setSize (MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    setName ("MainPanel");
    
    mTopPanel = std::make_unique<RBDTopPanel> (inProcessor);
    mTopPanel->setTopLeftPosition (0, 0);
    addAndMakeVisible (mTopPanel.get());
    
    mInputGainPanel = std::make_unique<RBDGainPanel> (inProcessor);
    mInputGainPanel->setTopLeftPosition (0, TOP_PANEL_HEIGHT);
    mInputGainPanel->setParameterID (kParameter_InputGain);
    addAndMakeVisible (mInputGainPanel.get());
    
    mOutputGainPanel = std::make_unique<RBDGainPanel> (inProcessor);
    mOutputGainPanel->setTopLeftPosition (MAIN_PANEL_WIDTH - GAIN_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    mOutputGainPanel->setParameterID (kParameter_OutputGain);
    addAndMakeVisible (mOutputGainPanel.get());
    
    mCenterPanel = std::make_unique<RBDCenterPanel> (inProcessor);
    mCenterPanel->setTopLeftPosition (GAIN_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    addAndMakeVisible (mCenterPanel.get());
}

RBDMainPanel::~RBDMainPanel()
{
    
}
