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
    
    ~KAPLookAndFeel()
    {
        
    }
    
//  BUTTONS
    
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
    
//  COMBOBOXES
    Font getLabelFont (Label& label) override
    {
        return KAP::font1;
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted,
                            bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* passedTextColour) override
    {
        Rectangle<int> bounds (area);       // create editable copy of passed bounds to adjust them
        bounds.removeFromBottom (1);
        Colour fillColour = isHighlighted ? KAP::colour5 : KAP::colour4;
        
        g.setColour (fillColour);
        g.fillRect (bounds);
        
        Colour textColour = isTicked ? KAP::colour7 : KAP::colour1;
        g.setColour (textColour);
        g.setFont (KAP::font1);
        
        // TODO: check the bounds for the text - do they make sense?
        bounds.setLeft (10);
        bounds.setY (1);
        
        g.drawFittedText (text, bounds, Justification::left, 1);
    }
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& comboBox) override
    {
        const float cornerSize = 3.0f;
        const Rectangle<float> comboBoxBounds (0.0f, 0.0f, width, height);
        
        g.setColour (KAP::colour3);
        g.fillRoundedRectangle (comboBoxBounds, cornerSize);
        
        const Rectangle<float> arrowBounds (comboBoxBounds.withWidth (20.0f).withRightX (width - 10.0f));
        
        // Create arrow shape
        Path arrow;
        arrow.startNewSubPath (arrowBounds.getX() + 3.0f, arrowBounds.getCentreY() - 2.0f);
        arrow.lineTo (arrowBounds.getCentreX(), arrowBounds.getCentreY() + 2.0f);
        arrow.lineTo (arrowBounds.getRight() - 3.0f, arrowBounds.getCentreY() - 2.0f);
        
        // TODO: what does the conditional here achieve? Comment or fix
        const Colour arrowColour = comboBox.findColour (ComboBox::arrowColourId)
                                            .withAlpha (comboBox.isEnabled() ? 0.9f : 0.2f);
        g.setColour (arrowColour);
        g.strokePath (arrow, PathStrokeType (2.0f));
    }
    
private:
    
};