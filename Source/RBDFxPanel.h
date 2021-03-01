/*
  ==============================================================================

    RBDFxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDParameterSlider.h"
#include "RBDParameters.h"

class FxPanel  : public InterfacePanel,
                    public ComboBox::Listener
{
public:
    FxPanel (ReallyBasicDelayAudioProcessor& processor);
    ~FxPanel();
    
    void paint (Graphics& g) override;
    
    void setFxPanelStyle (FxType::Index typeIndex);
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
private:
    FxType::Index mTypeIndex;
    
    // Use an array or pointers to allow dynamically changing number of sliders
    OwnedArray<ParameterSlider> mSliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
