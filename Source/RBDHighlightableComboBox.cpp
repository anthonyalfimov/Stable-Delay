/*
  ==============================================================================

    RBDHighlightableComboBox.cpp
    Created: 10 Mar 2021 10:36:46pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDHighlightableComboBox.h"
#include "RBDInterfaceConstants.h"

HighlightableComboBox::HighlightableComboBox (const String& componentName)
    : ComboBox (componentName)
{
    
}

HighlightableComboBox::~HighlightableComboBox()
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

