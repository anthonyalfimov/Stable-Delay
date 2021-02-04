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
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);
    setName ("MainPanel");
    
    mTopPanel = std::make_unique<RBDTopPanel> (inProcessor);
    mTopPanel->setTopLeftPosition (0, 0);
    addAndMakeVisible (mTopPanel.get());
    
    mInputGainPanel = std::make_unique<RBDGainPanel> (inProcessor);
    mInputGainPanel->setTopLeftPosition (0, mTopPanel->getBottom());
    mInputGainPanel->setParameterID (kParameter_InputGain);
    addAndMakeVisible (mInputGainPanel.get());
    
    mCenterPanel = std::make_unique<RBDCenterPanel> (inProcessor);
    mCenterPanel->setTopLeftPosition (mInputGainPanel->getRight(), mTopPanel->getBottom());
    addAndMakeVisible (mCenterPanel.get());
    
    mOutputGainPanel = std::make_unique<RBDGainPanel> (inProcessor);
    mOutputGainPanel->setTopRightPosition (getRight(), mTopPanel->getBottom());
    mOutputGainPanel->setParameterID (kParameter_OutputGain);
    addAndMakeVisible (mOutputGainPanel.get());
}

RBDMainPanel::~RBDMainPanel()
{
    
}
