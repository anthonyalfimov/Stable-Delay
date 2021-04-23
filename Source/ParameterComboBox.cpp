/*
  ==============================================================================

    ParameterComboBox.cpp
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#include "ParameterComboBox.h"

ParameterComboBox::ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                                      Parameter::Index parameterIndex)
    : HighlightableComboBox (Parameter::Name[parameterIndex])
{
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    mAttachment = std::make_unique<ComboBoxAttachment> (stateToControl,
                                                        Parameter::ID[parameterIndex],
                                                        *this);
}

ParameterComboBox::ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                                      Parameter::Index parameterIndex,
                                      const StringArray& itemList,
                                      int firstItemIdOffset)
    : HighlightableComboBox (Parameter::Name[parameterIndex])
{
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    addItemList (itemList, firstItemIdOffset);

    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    mAttachment = std::make_unique<ComboBoxAttachment> (stateToControl,
                                                        Parameter::ID[parameterIndex],
                                                        *this);
}

ParameterComboBox::~ParameterComboBox()
{
    
}
