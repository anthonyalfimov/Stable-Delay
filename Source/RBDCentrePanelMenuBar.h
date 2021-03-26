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
#include "RBDParameterSlider.h"
#include "RBDSliderLabel.h"

class CentrePanelMenuBar  : public InterfacePanel
{
public:
    CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor);
    ~CentrePanelMenuBar();
    
    // These are here because mFxTypeComboBox is a private member of this class
    /** @internal */
    void addFxTypeComboBoxListener (ComboBox::Listener* inListener);
    /** @internal */
    void removeFxTypeComboBoxListener (ComboBox::Listener* inListener);
    
private:
    std::unique_ptr<ParameterComboBox> mFxTypeComboBox;
    std::unique_ptr<ParameterSlider> mWidthSlider;
    std::unique_ptr<SliderLabel> mWidthLabel;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentrePanelMenuBar)
};
