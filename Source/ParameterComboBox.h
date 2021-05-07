/*
  ==============================================================================

    ParameterComboBox.h
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "MouseEventInvoker.h"
#include "Parameters.h"

class ParameterComboBox  : public ComboBox
{
public:
    ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                       Parameter::Index parameterIndex,
                       const StringArray& itemList,
                       int firstItemIdOffset = 1);
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;
    
//==============================================================================
    MouseEventInvoker mMouseEventInvoker;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterComboBox)
};
