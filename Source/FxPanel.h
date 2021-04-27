/*
  ==============================================================================

    FxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "ParameterKnob.h"
#include "SliderLabel.h"
#include "Parameters.h"

class FxPanel  : public InterfacePanel,
                 public ComboBox::Listener
{
public:
    explicit FxPanel (ReallyBasicDelayAudioProcessor& processor);

//==============================================================================
    void setFxPanelStyle (FxType::Index typeIndex);

//==============================================================================
    /** @internal */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
private:
    FxType::Index mTypeIndex;
    
    // Use an array of pointers to allow dynamically changing number of knobs
    OwnedArray<ParameterKnob> mKnobs;

    Label mFxTypeLabel { "fxType", "ERROR" };

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
