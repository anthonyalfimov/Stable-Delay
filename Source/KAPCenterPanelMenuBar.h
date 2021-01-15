/*
  ==============================================================================

    KAPCenterPanelMenuBar.h
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"
#include "KAPParameterComboBox.h"

class KAPCenterPanelMenuBar  : public KAPPanelBase
{
public:
    KAPCenterPanelMenuBar (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPCenterPanelMenuBar();
    
    // These are here because mFxTypeComboBox is a private member of this class
    void addFxTypeComboBoxListener (ComboBox::Listener* inListener);
    void removeFxTypeComboBoxListener (ComboBox::Listener* inListener);
    
private:
    std::unique_ptr<KAPParameterComboBox> mFxTypeComboBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPCenterPanelMenuBar)
};
