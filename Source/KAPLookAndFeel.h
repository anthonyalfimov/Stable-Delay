/*
  ==============================================================================

    KAPLookAndFeel.h
    Created: 25 Jan 2021 7:34:49pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "KAPInterfaceDefines.h"

class KAPLookAndFeel  : public LookAndFeel_V4
{
public:
    KAPLookAndFeel()
    {
        // ComboBox colours
        setColour (ComboBox::backgroundColourId, KAP::colour3);
        setColour (ComboBox::outlineColourId, KAP::colour2);
        setColour (ComboBox::arrowColourId, KAP::colour1);
        setColour (ComboBox::textColourId, KAP::colour1);
        
        // TextButton colours
        setColour (TextButton::buttonColourId, KAP::colour1);
        setColour (TextButton::textColourOnId, KAP::colour1);
        setColour (TextButton::textColourOffId, KAP::colour1);
    }
    
    ~KAPLookAndFeel();
    
    Font getTextButtonFont (TextButton& button, int buttonHeight) override
    {
        return KAP::font1;
    }
    
    void drawButtonBackground (Graphics& g, Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        Colour fillColour;  // FIXME: shouldn't we initialise it to the non-highlighted, non-down colour?
        
        if (shouldDrawButtonAsDown)
        {
            fillColour = KAP::colour5;
        }
        else if (shouldDrawButtonAsHighlighted)
        {
            fillColour = KAP::colour3;
        }
        
        const float cornerSize = 6.0f;
        
        // TODO: non-namespaced Rectangle might be a problem on Windows, test it and fix if it is
        const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced (0.5f);
        
        g.setColour (fillColour);
        g.fillRoundedRectangle (bounds.reduced (1.0f), cornerSize);
    }
    
private:
    
};
