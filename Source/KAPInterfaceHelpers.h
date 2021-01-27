/*
  ==============================================================================

    KAPInterfaceHelpers.h
    Created: 14 Jan 2021 9:06:14pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "KAPInterfaceDefines.h"

// TODO: Implement labels using JUCE's Label class

inline void paintComponentLabel (Graphics& g, Component* inComponent)
{
    const int width = inComponent->getWidth() * 1.5;
    const int height = 20;
    const int yPosition = inComponent->getBottom();
    auto textBounds = inComponent->getBounds().withSizeKeepingCentre (width, height)
                                              .withY (yPosition);
    
    g.setColour (KAP::colour3);
    g.fillRoundedRectangle (textBounds.toFloat(), KAP::defaultCornerSize);
    
    const String label = inComponent->getName();
    g.setColour (KAP::colour1);
    g.setFont (KAP::font1);
    g.drawFittedText (label, textBounds, Justification::centred, 1);
}
