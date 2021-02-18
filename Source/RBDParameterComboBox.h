/*
  ==============================================================================

    RBDParameterComboBox.h
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDParameters.h"

class ParameterComboBox  : public ComboBox
{
public:
    ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                       Parameter::Index parameterIndex);
    ~ParameterComboBox();
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterComboBox)
};
