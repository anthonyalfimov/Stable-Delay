/*
  ==============================================================================

    RBDFxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDParameterKnob.h"
#include "RBDSliderLabel.h"
#include "RBDParameters.h"

class FxPanel  : public InterfacePanel,
                 public ComboBox::Listener
{
public:
    FxPanel (ReallyBasicDelayAudioProcessor& processor);
    ~FxPanel();
    
    void setFxPanelStyle (FxType::Index typeIndex);
    
    /** @internal */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
private:
    FxType::Index mTypeIndex;
    
    // Use an array or pointers to allow dynamically changing number of knobs
    OwnedArray<ParameterKnob> mKnobs;
    OwnedArray<SliderLabel> mLabels;

    Label mFxTypeLabel { "fxType", "ERROR" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
