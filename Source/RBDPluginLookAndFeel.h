/*
  ==============================================================================

    RBDPluginLookAndFeel.h
    Created: 25 Jan 2021 7:34:49pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDInterfaceConstants.h"

// TODO: Move towards using the LookAndFeel facilities more
//  E.g. when drawing a button, the text could be drawn using Label painting
//  method. A lot of methods that we override have parameters that we don't use,
//  but, perhaps, should.

class PluginLookAndFeel  : public LookAndFeel_V4
{
public:
    PluginLookAndFeel();
    
    ~PluginLookAndFeel();

//= LABELS =====================================================================

     void drawLabel (Graphics& g, Label& label) override;
    
//= BUTTONS ====================================================================
    
    Font getTextButtonFont (TextButton& button, int buttonHeight) override;
    
    void drawButtonBackground (Graphics& g, Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

//= POPUP MENUS ================================================================

    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive,
                            bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon,
                            const Colour* passedTextColour) override;

//= COMBOBOXES =================================================================
    // TODO: Make the popup list semi-transparent, with rounded corners

    Font getComboBoxFont (ComboBox& comboBox) override;
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& comboBox) override;
    
//= SLIDERS ====================================================================

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override;
    
private:
    // NB! Image object dynamically allocates memory for the image data,
    //  so we create it as a contained object, not a pointer!
    Image mKnobImage;
};
