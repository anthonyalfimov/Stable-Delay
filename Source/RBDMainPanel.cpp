/*
  ==============================================================================

    RBDMainPanel.cpp
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDMainPanel.h"
#include "RBDParameters.h"

MainPanel::MainPanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : InterfacePanel (inProcessor)
{
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);
    setName ("MainPanel");
    
    mTopPanel = std::make_unique<TopPanel> (inProcessor);
    mTopPanel->setTopLeftPosition (0, 0);
    addAndMakeVisible (mTopPanel.get());
    
    mInputGainPanel = std::make_unique<GainPanel> (inProcessor);
    mInputGainPanel->setTopLeftPosition (0, mTopPanel->getBottom());
    mInputGainPanel->setParameterID (Parameter::InputGain);
    addAndMakeVisible (mInputGainPanel.get());
    
    mCentrePanel = std::make_unique<CentrePanel> (inProcessor);
    mCentrePanel->setTopLeftPosition (mInputGainPanel->getRight(), mTopPanel->getBottom());
    addAndMakeVisible (mCentrePanel.get());
    
    mOutputGainPanel = std::make_unique<GainPanel> (inProcessor);
    mOutputGainPanel->setTopRightPosition (getRight(), mTopPanel->getBottom());
    mOutputGainPanel->setParameterID (Parameter::OutputGain);
    addAndMakeVisible (mOutputGainPanel.get());
}

MainPanel::~MainPanel()
{
    
}
