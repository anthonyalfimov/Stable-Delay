/*
  ==============================================================================

    RBDParameterComboBox.h
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RBDParameterComboBox  : public ComboBox
{
public:
    RBDParameterComboBox (AudioProcessorValueTreeState& stateToControl, const String& parameterID);
    ~RBDParameterComboBox();
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDParameterComboBox)
};
