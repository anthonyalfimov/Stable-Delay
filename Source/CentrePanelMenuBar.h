/*
  ==============================================================================

    CenterPanelMenuBar.h
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "ParameterComboBox.h"
#include "ParameterSlider.h"
#include "SliderLabel.h"

class CentrePanelMenuBar  : public InterfacePanel
{
public:
    explicit CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor);

//==============================================================================
    // These are here because mFxTypeComboBox is a private member of this class
    /** @internal */
    void addFxTypeComboBoxListener (ComboBox::Listener* inListener);
    /** @internal */
    void removeFxTypeComboBoxListener (ComboBox::Listener* inListener);
    
private:
    std::unique_ptr<ParameterComboBox> mFxTypeComboBox;
    std::unique_ptr<ParameterSlider> mSpreadSlider;
    std::unique_ptr<SliderLabel> mSpreadLabel;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentrePanelMenuBar)
};
