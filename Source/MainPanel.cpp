/*
  ==============================================================================

    MainPanel.cpp
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#include "MainPanel.h"
#include "Parameters.h"

MainPanel::MainPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);
    setName ("MainPanel");
    
    mTopPanel = std::make_unique<TopPanel> (processor);
    mTopPanel->setTopLeftPosition (0, 0);
    addAndMakeVisible (mTopPanel.get());
    
    mInputGainPanel = std::make_unique<GainPanel> (processor, Parameter::InputGain);
    mInputGainPanel->setTopLeftPosition (0, mTopPanel->getBottom());
    addAndMakeVisible (mInputGainPanel.get());
    
    mCentrePanel = std::make_unique<CentrePanel> (processor);
    mCentrePanel->setTopLeftPosition (mInputGainPanel->getRight(),
                                      mTopPanel->getBottom());
    addAndMakeVisible (mCentrePanel.get());
    
    mOutputGainPanel = std::make_unique<GainPanel> (processor, Parameter::OutputGain);
    mOutputGainPanel->setTopRightPosition (getRight(), mTopPanel->getBottom());
    addAndMakeVisible (mOutputGainPanel.get());
}

MainPanel::~MainPanel()
{
    
}
