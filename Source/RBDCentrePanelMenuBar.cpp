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
    
    mFxTypeComboBox = std::make_unique<ParameterComboBox> (mProcessor->parameters,
                                                              RBDParameterID[kParameter_DelayType]);
    const int width = 90;
    mFxTypeComboBox->setBounds (getWidth() - width, 0, width, getHeight());
    
    // TODO: Clean up usage of RBDFxPanelStyle, ComboBox index and ID
    // TODO: replace IDs with an enum class
    // NB: IDs are not the same as indices, and IDs must start with 1
    mFxTypeComboBox->addItem ("DELAY", 1);
    mFxTypeComboBox->addItem ("CHORUS", 2);
    auto& parameters = mProcessor->getParameters();
    // getUnchecked() is faster, but doesn't check the passed index. This is UI, so why not?
    const int selectedItemIndex = static_cast<int> (parameters[kParameter_DelayType]->getValue());
    mFxTypeComboBox->setSelectedItemIndex (selectedItemIndex, dontSendNotification);
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
