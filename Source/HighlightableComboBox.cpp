/*
  ==============================================================================

    HighlightableComboBox.cpp
    Created: 10 Mar 2021 10:36:46pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "HighlightableComboBox.h"
#include "InterfaceConstants.h"

HighlightableComboBox::HighlightableComboBox (const String& componentName)
    : ComboBox (componentName)
{
    
}

//==============================================================================
void HighlightableComboBox::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void HighlightableComboBox::mouseExit (const MouseEvent& event)
{
    repaint();
}

void HighlightableComboBox::mouseUp (const MouseEvent& event)
{
    repaint();
}

