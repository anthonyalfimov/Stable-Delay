/*
  ==============================================================================

    RBDCenterPanelMenuBar.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCentrePanelMenuBar.h"
#include "RBDParameters.h"

CentrePanelMenuBar::CentrePanelMenuBar (ReallyBasicDelayAudioProcessor* inProcessor)
    : InterfacePanel (inProcessor)
{
    setSize (RBD::centrePanelMenuBarWidth, RBD::centrePanelMenuBarHeight);
    setName ("CenterMenu");
    
    mFxTypeComboBox
    = std::make_unique<ParameterComboBox> (mProcessor->parameters, Parameter::DelayType);
    const int width = 90;
    mFxTypeComboBox->setBounds (getWidth() - width, 0, width, getHeight());
    
    mFxTypeComboBox->addItem ("DELAY", static_cast<int> (FxTypeID::Delay));
    mFxTypeComboBox->addItem ("CHORUS", static_cast<int> (FxTypeID::Chorus));
    auto& parameters = mProcessor->getParameters();
    
    const auto selectedTypeID
    = floatToFxTypeID (parameters[Parameter::DelayType]->getValue());
    mFxTypeComboBox->setSelectedId (static_cast<int> (selectedTypeID),
                                    dontSendNotification);
    addAndMakeVisible (mFxTypeComboBox.get());
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
