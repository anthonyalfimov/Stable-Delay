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
    /**
        Create a ComboBox that will be attached to the parameter in
        stateToControl that is identified by the parameterIndex.

        @Discussion
        Initial ComboBox update happens when attachment is created, so all
        ComboBox items must be supplied to the constructor. The constructor
        adds the supplied items to the ComboBox, and then creates the parameter
        attachment.
    */
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
