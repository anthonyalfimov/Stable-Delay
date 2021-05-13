/*
  ==============================================================================

    ParameterComboBox.cpp
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#include "ParameterComboBox.h"
#include "InterfaceConstants.h"

ParameterComboBox::ParameterComboBox (AudioProcessorValueTreeState& stateToControl,
                                      Parameter::Index parameterIndex,
                                      const StringArray& itemList,
                                      int firstItemIdOffset)
    : ComboBox (Parameter::Name[parameterIndex])
{
    // Set up item list
    addItemList (itemList, firstItemIdOffset);

    // Set up parameter attachment
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    mAttachment = std::make_unique<ComboBoxAttachment> (stateToControl,
                                                        Parameter::ID[parameterIndex],
                                                        *this);

    // Set up mouse event callbacks
    auto repaintComboBox = [this] (const MouseEvent& event) { repaint(); };

    mMouseEventInvoker.onMouseEnter = repaintComboBox;
    mMouseEventInvoker.onMouseExit = repaintComboBox;
    mMouseEventInvoker.onMouseUp = repaintComboBox;

    addMouseListener (&mMouseEventInvoker, true);

    // Set up style
    setColour (ComboBox::textColourId, RBD::textFxTypeColour);
}
