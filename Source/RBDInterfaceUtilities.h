/*
  ==============================================================================

    RBDInterfaceHelpers.h
    Created: 14 Jan 2021 9:06:14pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDInterfaceConstants.h"

// TODO: Implement labels using JUCE's Label class

inline void paintComponentLabel (Graphics& g, Component* inComponent)
{
    const int width = inComponent->getWidth() * 1;
    const int height = RBD::labelHeight;
    const int yPosition = inComponent->getBottom();
    auto textBounds = inComponent->getBounds().withSizeKeepingCentre (width, height)
                                              .withY (yPosition);
    
    // TODO: make the label colour change when hovering over "parent" component
    const Colour labelBgColour = inComponent->isMouseOver() ? RBD::colour6 : RBD::colour3;
    g.setColour (labelBgColour);
    g.fillRoundedRectangle (textBounds.toFloat(), RBD::defaultCornerSize);
    
    const String label = inComponent->getName();
    g.setColour (RBD::colour1);
    g.setFont (RBD::font1);
    g.drawFittedText (label, textBounds, Justification::centred, 1);
}
