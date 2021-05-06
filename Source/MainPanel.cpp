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
    // Set up Panel attributes
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);
    setName ("MainPanel");

    // TODO: Split the main panel between sub-panels usign removeFromTop(), etc.
    //  Define positions and sizes of all UI panels by subdividing the main
    //  panel. This will reduce the number of interface size constants we need
    //  and prepare the UI to be scalable. Aim at using relative sizes for the
    //  size constants.
    //
    //  NB! This will require to change most of the UI panels so that they don't
    //  position their elements in their constructors, but instead do this in
    //  the resized() method!

    // Set up Top Panel
    mTopPanel = std::make_unique<TopPanel> (processor);
    mTopPanel->setTopLeftPosition (0, 0);
    addAndMakeVisible (mTopPanel.get());
    mAboutPanelTrigger = mTopPanel->addTitleMouseListener (this);

    // Set up Input Panel
    mInputGainPanel = std::make_unique<GainPanel> (processor, Parameter::InputGain);
    mInputGainPanel->setTopLeftPosition (0, mTopPanel->getBottom());
    addAndMakeVisible (mInputGainPanel.get());

    // Set up Centre Panel
    mCentrePanel = std::make_unique<CentrePanel> (processor);
    mCentrePanel->setTopLeftPosition (mInputGainPanel->getRight(),
                                      mTopPanel->getBottom());
    addAndMakeVisible (mCentrePanel.get());

    // Set up Output Panel
    mOutputGainPanel = std::make_unique<GainPanel> (processor, Parameter::OutputGain);
    mOutputGainPanel->setTopRightPosition (getRight(), mTopPanel->getBottom());
    addAndMakeVisible (mOutputGainPanel.get());

    // Load background image
    mBackgroundImage = ImageCache::getFromMemory (BinaryData::RBD_BG_2x_png,
                                                  BinaryData::RBD_BG_2x_pngSize);
}

MainPanel::~MainPanel()
{
    mTopPanel->removeTitleMouseListener (this);
}

void MainPanel::paint (Graphics& g)
{
    // Draw background image
    g.setOpacity (1.0f);    // Make sure the image is drawn opaque
    g.drawImage (mBackgroundImage, getLocalBounds().toFloat());
}

void MainPanel::mouseDown (const MouseEvent& event)
{
    if (event.eventComponent != mAboutPanelTrigger)
        return;

    mAboutPanel = std::make_unique<AboutPanel> (mAboutPanel);
    mAboutPanel->setBounds (getLocalBounds());
    addAndMakeVisible (mAboutPanel.get());
}
