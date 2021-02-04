/*
  ==============================================================================

    RBDParameterComboBox.cpp
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDParameterComboBox.h"

ParameterComboBox::ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                                      const String& parameterID)
    : ComboBox (parameterID)
{
    mAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment> (stateToControl,
                                                                                      parameterID,
                                                                                      *this);
}

ParameterComboBox::~ParameterComboBox()
{
    
}
