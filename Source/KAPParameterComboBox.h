/*
  ==============================================================================

    KAPParameterComboBox.h
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KAPParameterComboBox  : public ComboBox
{
public:
    KAPParameterComboBox (AudioProcessorValueTreeState& stateToControl, const String& parameterID);
    ~KAPParameterComboBox();
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
};
