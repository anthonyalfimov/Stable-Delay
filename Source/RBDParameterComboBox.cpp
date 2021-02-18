/*
  ==============================================================================

    RBDParameterComboBox.cpp
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDParameterComboBox.h"

ParameterComboBox::ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                                      Parameter::Index parameterIndex)
    : ComboBox (Parameter::Label[parameterIndex])
{
    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    mAttachment = std::make_unique<ComboBoxAttachment> (stateToControl,
                                                        Parameter::ID[parameterIndex],
                                                        *this);
}

ParameterComboBox::~ParameterComboBox()
{
    
}
