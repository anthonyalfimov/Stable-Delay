/*
  ==============================================================================

    ClickableLabel.cpp
    Created: 6 May 2021 10:57:24pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "ClickableLabel.h"

ClickableLabel::ClickableLabel (const String& componentName,
                                const String& labelText)
    : Label (componentName, labelText)
{
    setEditable (false);
}

void ClickableLabel::mouseDown (const MouseEvent& /*event*/)
{
    if (onClick != nullptr)
        onClick();
}
