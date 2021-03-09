/*
  ==============================================================================

    RBDCenterPanelMenuBar.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCentrePanelMenuBar.h"
#include "RBDParameters.h"

CentrePanelMenuBar::CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::centrePanelMenuBarWidth, RBD::centrePanelMenuBarHeight);
    setName ("CenterMenu");

    // Set up FX Type ComboBox

    // Note: Initial ComboBox update happens when attachment is created
    //  Using the ParameterComboBox ctor that takes the item list allows to
    //  add the items before the attachment is created and ComboBox is updated

    mFxTypeComboBox = std::make_unique<ParameterComboBox> (mProcessor.parameters,
                                                           Parameter::FxType,
                                                           FxType::Label);
    const int width = 95;
    auto bounds = getLocalBounds().withLeft (getWidth() - width);
    mFxTypeComboBox->setBounds (bounds);
    addAndMakeVisible (mFxTypeComboBox.get());

    // TODO: Enable build stamp only for the debug version, or remove later
    // Set up Build Version Label
    mBuildVersionLabel.setFont (RBD::mainFont);
    mBuildVersionLabel.setJustificationType (Justification::centredLeft);
    const auto buildDate = Time::getCompilationDate();
    mBuildVersionLabel.setText ("Build: " + String (buildDate.getHours()) + ":"
                                + String (buildDate.getMinutes()) + ", "
                                + String (buildDate.getDayOfMonth()) + " "
                                + buildDate.getMonthName (true) + " "
                                + String (buildDate.getYear()),
                                dontSendNotification);
    mBuildVersionLabel.setBorderSize ({ 1, 10, 1, 10 });
    bounds.setX (0);
    bounds.setRight (mFxTypeComboBox->getX());
    mBuildVersionLabel.setBounds (bounds);
    addAndMakeVisible (mBuildVersionLabel);
}

CentrePanelMenuBar::~CentrePanelMenuBar()
{
    
}

void CentrePanelMenuBar::addFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->addListener (inListener);
}

void CentrePanelMenuBar::removeFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->removeListener (inListener);
}
