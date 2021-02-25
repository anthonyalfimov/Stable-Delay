/*
  ==============================================================================

    RBDCenterPanelMenuBar.h
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDParameterComboBox.h"

class CentrePanelMenuBar  : public InterfacePanel
{
public:
    CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor);
    ~CentrePanelMenuBar();

    void paint (Graphics& g) override;
    
    // These are here because mFxTypeComboBox is a private member of this class
    void addFxTypeComboBoxListener (ComboBox::Listener* inListener);
    void removeFxTypeComboBoxListener (ComboBox::Listener* inListener);
    
private:
    std::unique_ptr<ParameterComboBox> mFxTypeComboBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentrePanelMenuBar)
};
