/*
  ==============================================================================

    PluginLookAndFeel.h
    Created: 25 Jan 2021 7:34:49pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "InterfaceConstants.h"

// TODO: Use custom LookAndFeel class to handle all painting in the plugin
//  Go beyond the already defined drawing methods of LookAndFeel_V4 and add
//  new ones for custom component classes of the plugin like meters, etc.
//  Copy the pattern that L&F classes use with standard JUCE components.
//  When all painting is handled by L&F, the interface and colour constants
//  should reside inside this class as well.
//  A lot of paint methods that we override have parameters that we don't use,
//  but, perhaps, should.

class PluginLookAndFeel  : public LookAndFeel_V4
{
public:
    /**
        Creates PluginLookAndFeel objects and intialises its values and assets.
    */
    PluginLookAndFeel();

//= UTILITIES ==================================================================

    // Offsets to visually center centered text that uses largeFont
    inline static const int largeFontRightOffset = 1;
    inline static const int largeFontBottomOffset = 2;

    void drawUpDownArrowButton (Graphics& g, Colour arrowColour,
                                bool isUpArrow,
                                int buttonX, int buttonY,
                                int buttonW, int buttonH);

//= LABELS =====================================================================

     void drawLabel (Graphics& g, Label& label) override;
    
//= BUTTONS ====================================================================

    inline static const int largeButtonMinHeight = 45;

    void drawButtonBackground (Graphics& g, Button& button,
                               const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    Font getTextButtonFont (TextButton& button, int buttonHeight) override;

    void drawButtonText (Graphics& g, TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;

//= POPUP MENUS ================================================================

    inline static const int popupMenuItemHeight = 24;

    void drawPopupMenuBackground (Graphics& g, int width, int height) override;

    Font getPopupMenuFont() override;

    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive,
                            bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon,
                            const Colour* textColourToUse) override;

    void drawPopupMenuUpDownArrow (Graphics& g, int width, int height,
                                   bool isScrollUpArrow) override;

    void getIdealPopupMenuItemSize (const String& text, bool isSeparator,
                                    int standardMenuItemHeight,
                                    int& idealWidth, int& idealHeight) override;

//= COMBOBOXES =================================================================

    inline static const int comboBoxButtonWidth = 40;
    inline static const int largeComboBoxMinHeight = 40;

    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& comboBox) override;

    Font getComboBoxFont (ComboBox& comboBox) override;

    void positionComboBoxText (ComboBox& comboBox, Label& label) override;

    PopupMenu::Options getOptionsForComboBoxPopupMenu (ComboBox& comboBox,
                                                       Label& label) override;
    
//= SLIDERS ====================================================================

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override;
    
private:
//= SLIDERS ====================================================================

    // NB! Image object dynamically allocates memory for the image data,
    //  so we create it as a contained object, not a pointer!
    Image mKnobImage;

    inline static const float knobMarkThickness = 2.3f;
    inline static const float knobMarkStart = 7.0f;
    inline static const float knobMarkEnd = 16.0f;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginLookAndFeel)
};
