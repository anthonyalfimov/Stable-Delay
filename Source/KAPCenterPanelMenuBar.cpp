/*
  ==============================================================================

    KAPCenterPanelMenuBar.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPCenterPanelMenuBar.h"
#include "KAPParameters.h"

KAPCenterPanelMenuBar::KAPCenterPanelMenuBar (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (CENTER_PANEL_MENU_BAR_WIDTH, CENTER_PANEL_MENU_BAR_HEIGHT);
    setName ("CenterMenu");
    
    mFxTypeComboBox = std::make_unique<KAPParameterComboBox> (mProcessor->parameters,
                                                              KAPParameterID[kParameter_DelayType]);
    const int width = 85;
    mFxTypeComboBox->setBounds (getWidth() - width, 0, width, getHeight());
    
    // TODO: Clean up usage of KAPFxPanelStyle, ComboBox index and ID
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

KAPCenterPanelMenuBar::~KAPCenterPanelMenuBar()
{
    
}

void KAPCenterPanelMenuBar::addFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->addListener (inListener);
}

void KAPCenterPanelMenuBar::removeFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->removeListener (inListener);
}
